#include <stdlib.h>
#include <string.h>

#include "../Headers/Common.h"
#include "../Headers/Compiler.h"
#include "../Headers/CompilerErrors.h"
#include "../Headers/Scanner.h"
#include "../Headers/Value.h"
#include "../Headers/Token.h"
#include "../Headers/Bytecode.h"
#include "../Headers/Object.h"
#include "../Headers/CompilerBytecode.h"
#include "../Headers/Local.h"
#include "../Headers/Helpers.h"
#include "../Headers/Memory.h"
#include "../Headers/Debug.h"

static void InitCompiler(Compiler* compiler, FunctionType functionType) {
    compiler->function = NULL;
    compiler->functionType = functionType;
    compiler->localCount = 0;
    compiler->scopeDepth = 0;
    compiler->function = NewFunction();
    compiler->enclosing = currentCompiler;
    currentCompiler = compiler;
    
    if (functionType != TYPE_SCRIPT) {
        currentCompiler->function->name = CopyString(parser.previous.start, parser.previous.length);
    }
    
    Local* local = &compiler->locals[compiler->localCount++];
    local->depth = 0;
    local->name.start = "";
    local->name.length = 0;
}

static ObjFunction* EndCompiler() {
    WriteReturn();
    currentCompiler = currentCompiler->enclosing;
#ifdef DEBUG_PRINT_CODE
    if (!parser.hadError) {
        DisassembleBytecode(compiler.function.bytecode, compiler.function->name != NULL ? compiler.function->name->chars : "<script>");
    }
#endif
    return currentCompiler->function;
}

static void InitParser() {
    parser.hadError = false;
}

static ParseRule* GetRule(TokenType type);


static void GlobalStatement() {
    do {
        uint8_t address = StoreConstant(OBJ_VAL(CopyString(parser.current.start, parser.current.length)));
        WriteBytes(DEFINE_GLOBAL_OP, address);
        GetNextToken();
    } while (MatchToken(COMMA_TOKEN));
}

static void Expression() {
    ParsePrecedence(PREC_ASSIGNMENT);
}

static void StarExpression() {
    Expression();
}

static void LambdaDef() {
    
}

static void Test() {
    if (MatchToken(LAMBDA_TOKEN)) {
        LambdaDef();
    } else {
        ParsePrecedence(PREC_ASSIGNMENT);
    }
    if (MatchToken(IF_TOKEN)) {
        ParsePrecedence(PREC_OR);
        if (MatchToken(IF_TOKEN)) {
            Test();
        }
    }
}

static void TestListStarExpression() {
    do {
        if (MatchToken(STAR_TOKEN)) {
            StarExpression();
        }
        else {
            Test();
        }
    }
    while (MatchToken(COMMA_TOKEN));
}

static void ExpressionStatement() {
    TestListStarExpression();
    
    if(MatchToken(COLON_TOKEN)) {
        Test();
    }
    if(MatchToken(EQUAL_TOKEN)) {
        ParsePrecedence(PREC_ASSIGNMENT);
    }
}

static void SmallStatement() {
  
    //DelStatement();
    //PassStatement();
    //FlowStatement();
    //ImportStatement();
    if (MatchToken(GLOBAL_TOKEN)) {
        GlobalStatement();
    }
    else {
        ExpressionStatement();
    }
    //NonlocalStatement();
    //AssertStatement();
}

static void SimpleStatement() {
    do {
        SmallStatement();
    } while (MatchToken(SEMICOLON_TOKEN));
}

static void Comparison() {
    Expression();
}

static void TestList() {
    do {
        Test();
    } while (MatchToken(COMMA_TOKEN));
}

static void NamedExpressionTest() {
    Test();
    if (MatchToken(COLON_EQUAL_TOKEN)) {
        Test();
    }
}

static void Statement(void);

static void Suite() {
    if (MatchToken(NEWLINE_TOKEN)) {
        ConsumeToken(INDENT_TOKEN, IndentError);
        do {
            Statement();
            TryConsumeToken(NEWLINE_TOKEN);
        } while(!MatchToken(DEDENT_TOKEN) && !MatchToken(EOF_TOKEN));
    }
    else {
        SimpleStatement();
    }
}

static void ExpressionList() {
    do {
        if (MatchToken(STAR_TOKEN)) {
            StarExpression();
        }
        else {
            Expression();
        }
    } while (MatchToken(COMMA_TOKEN));
}

static void ExceptClause() {
    Test();
    if (MatchToken(AS_TOKEN)) {
        ConsumeToken(NAME_TOKEN, NameError);
    }
}

static void VariableDeclaration() {
    uint8_t global = ParseVariable();

    if (MatchToken(EQUAL_TOKEN)) {
        Expression();
    }
    else {
        WriteByte(NONE_OP);
    }

    DefineVariable(global);
}

static uint8_t GetIdentifierAddress() {
    if (currentCompiler->scopeDepth > 0) {
        return (uint8_t) ResolveLocal(&parser.previous);
    }
    else {
        return IdentifierConstant(&parser.previous);
    }
}

static void IfStatement() {
    NamedExpressionTest();
    ConsumeToken(COLON_TOKEN, ColonError);
    
    int ifFalseOpcodeAddress = WriteJump(JUMP_IF_FALSE_OP);
    int elifTrueOpcodeAddress = 0;
    int elifFalseOpcodeAddress = 0;
    
    Suite();
    
    int ifTrueOpcodeAddress = WriteJump(JUMP_OP);
    PatchJump(ifFalseOpcodeAddress);
    
    while (MatchToken(ELIF_TOKEN)) {
        NamedExpressionTest();
        ConsumeToken(COLON_TOKEN, ColonError);
        elifFalseOpcodeAddress = WriteJump(JUMP_IF_FALSE_OP);
        Suite();
        
        elifTrueOpcodeAddress = WriteJump(JUMP_OP);
        PatchJump(elifFalseOpcodeAddress);
    }
    
    if (MatchToken(ELSE_TOKEN)) {
        ConsumeToken(COLON_TOKEN, ColonError);
        Suite();
    }
    
    PatchJump(ifTrueOpcodeAddress);
    if (elifTrueOpcodeAddress != 0) {
        PatchJump(elifTrueOpcodeAddress);
    }
}

static void WhileStatement() {
    int loopStart = currentCompiler->function->bytecode.count;
    
    NamedExpressionTest();
    ConsumeToken(COLON_TOKEN, ColonError);
    
    int ifFalseOpcodeAddress = WriteJump(JUMP_IF_FALSE_OP);
    
    Suite();
    WriteLoop(loopStart);
    PatchJump(ifFalseOpcodeAddress);
}

static void ForStatement() {
    GetNextToken();
    Token loopVariable = parser.previous;
    VariableDeclaration();
    ConsumeToken(IN_TOKEN, InError);
    GetNextToken();
    uint8_t identifierAddress = GetIdentifierAddress();
    ConsumeToken(COLON_TOKEN, ColonError);
    
    int loopStart = currentCompiler->function->bytecode.count;
    WriteBytes(END_OF_ARRAY_OP, identifierAddress);
    
    int ifTrueOpcodeAddress = WriteJump(JUMP_IF_TRUE_OP);
    WriteBytes(GET_INDEX_OP, identifierAddress);
    if (currentCompiler->scopeDepth > 0) {
        WriteBytes(SET_LOCAL_OP, (uint8_t) ResolveLocal(&loopVariable));
    }
    else {
        WriteBytes(SET_GLOBAL_OP, IdentifierConstant(&loopVariable));
    }
    Suite();
    WriteLoop(loopStart);
    PatchJump(ifTrueOpcodeAddress);
}

static void TryStatement() {
    ConsumeToken(COLON_TOKEN, ColonError);
    Suite();
    do {
        ConsumeToken(EXCEPT_TOKEN, ExceptError);
        ExceptClause();
        ConsumeToken(COLON_TOKEN, ColonError);
        Suite();
    } while (MatchToken(EXCEPT_TOKEN));
}

static void WithStatement() {
    
}

static void Function(FunctionType functionType) {
    Compiler compiler;
    InitCompiler(&compiler, TYPE_FUNCTION);
    BeginScope();
    
    ConsumeToken(LEFT_PAREN_TOKEN, LeftParenError);
    if (!CheckToken(RIGHT_PAREN_TOKEN)) {
        do {
            currentCompiler->function->arity++;
            if (currentCompiler->function->arity > 255) {
                Error("Cannot have more than 255 parameters");
            }
            
            uint8_t paramConstant = ParseVariable();
            DefineVariable(paramConstant);
        } while(MatchToken(COMMA_TOKEN));
    }
    ConsumeToken(RIGHT_PAREN_TOKEN, RightParenError);
    
    ConsumeToken(COLON_TOKEN, ColonError);
    Suite();
    
    ObjFunction* function = EndCompiler();
    WriteBytes(CONSTANT_OP, StoreConstant(OBJ_VAL(function)));
}

static void FunctionDefinition() {
    uint8_t global = ParseVariable();
    MarkInitialised();
    Function(TYPE_FUNCTION);
    DefineVariable(global);
}

static void ClassDefinition() {
    
}

static void PrintStatement() {
    NamedExpressionTest();
    WriteByte(PRINT_OP);
}

static bool CompoundStatement() {
    if (MatchToken(IF_TOKEN)) {
        IfStatement();
        return true;
    }
    else if (MatchToken(WHILE_TOKEN)) {
        WhileStatement();
        return true;
    }
    else if (MatchToken(FOR_TOKEN)) {
        ForStatement();
        return true;
    }
    else if (MatchToken(TRY_TOKEN)) {
        TryStatement();
        return true;
    }
    else if (MatchToken(WITH_TOKEN)) {
        WithStatement();
        return true;
    }
    else if (MatchToken(DEF_TOKEN)) {
        FunctionDefinition();
        return true;
    }
    else if (MatchToken(CLASS_TOKEN)) {
        ClassDefinition();
        return true;
    }
    else if (MatchToken(PRINT_TOKEN)) {
        PrintStatement();
        return true;
    }
    else {
        return false;
    }
}

static void Statement() {
    if (parser.current.type == NEWLINE_TOKEN) return;
    bool valid = CompoundStatement();
    if (!valid) {
        SimpleStatement();
    }
}

static void Unary(bool canAssign) {
    TokenType operatorType = parser.previous.type;
    
    ParsePrecedence(PREC_UNARY);
    
    switch(operatorType) {
        case BANG_TOKEN: WriteByte(NOT_OP); break;
        case MINUS_TOKEN: WriteByte(NEGATE_OP); break;
        default: return;
    }
}

static void Binary(bool canAssign) {
    TokenType operatorType = parser.previous.type;
    
    //Compile the right operand
    ParseRule* rule = GetRule(operatorType);
    ParsePrecedence((Precedence)rule->precedence+1);
    
    switch(operatorType) {
        case BANG_EQUAL_TOKEN: WriteBytes(EQUAL_OP, NOT_OP); break;
        case EQUAL_EQUAL_TOKEN: WriteByte(EQUAL_OP); break;
        case GREATER_TOKEN: WriteByte(GREATER_OP); break;
        case GREATER_EQUAL_TOKEN: WriteBytes(LESSER_OP, NOT_OP); break;
        case LESSER_TOKEN: WriteByte(LESSER_OP); break;
        case PLUS_TOKEN: WriteByte(ADD_OP); break;
        case MINUS_TOKEN: WriteByte(SUBTRACT_OP); break;
        case STAR_TOKEN: WriteByte(MULTIPLY_OP); break;
        case SLASH_TOKEN: WriteByte(DIVIDE_OP); break;
        case POWER_TOKEN: WriteByte(POWER_OP); break;
        case AND_TOKEN: WriteByte(AND_OP); break;
        case OR_TOKEN: WriteByte(OR_OP); break;
        case NOT_TOKEN: WriteByte(NOT_OP); break;
        default: return;
    }
    
}

static void Grouping(bool canAssign) {
    Expression();
    ConsumeToken(RIGHT_PAREN_TOKEN, RightParenError);
}

static void Number(bool canAssign) {
    double number = strtod(parser.previous.start, NULL);
    WriteConstantOperation(NUMBER_VAL(number));
}

static void String(bool canAssign) {
    char* string = ALLOCATE(char, parser.previous.length);
    string = memcpy(string, parser.previous.start, parser.previous.length);
    string[parser.previous.length] = '\0';
    uint8_t address = StoreConstant(OBJ_VAL(CopyString(parser.previous.start, parser.previous.length+1)));
    WriteBytes(CONSTANT_OP, address);
    GetNextToken();
}

static void Boolean(bool canAssign) {
    bool boolean = parser.previous.start;
    WriteConstantOperation(BOOLEAN_VAL(boolean));
}

static void LocalIdentifier(bool canAssign) {
    int arg = ResolveLocal(&parser.previous);
    
    if (arg == -1) {
        VariableDeclaration();
    }
    else {
        WriteBytes(GET_LOCAL_OP, (uint8_t)arg);
    }
}

static void GlobalIdentifier(bool canAssign) {
    int arg = IdentifierConstant(&parser.previous);
    
     if (canAssign && MatchToken(EQUAL_TOKEN)) {
           Expression();
           WriteBytes(SET_GLOBAL_OP, (uint8_t)arg);
       }
       else {
           WriteBytes(GET_GLOBAL_OP, (uint8_t)arg);
       }
}

static void Identifier(bool canAssign) {

    if (currentCompiler->scopeDepth > 0) {
        LocalIdentifier(canAssign);
    }
    else {
        GlobalIdentifier(canAssign);
    }
}

ParseRule rules[] = {
    {Grouping,   NULL,      PREC_CALL},         //      LEFT_PAREN_TOKEN
    {NULL,       NULL,      PREC_NONE},         //      RIGHT_PAREN_TOKEN
    {NULL,       NULL,      PREC_NONE},         //      LEFT_BRACE_TOKEN
    {NULL,       NULL,      PREC_NONE},         //      RIGHT_BRACE_TOKEN
    {NULL,       NULL,      PREC_NONE},         //      EQUAL_TOKEN
    {Unary,      NULL,      PREC_NONE},         //      BANG_TOKEN
    {NULL,       Binary,    PREC_EQUALITY},     //      BANG_EQUAL_TOKEN
    {NULL,       Binary,    PREC_EQUALITY},     //      EQUAL_EQUAL_TOKEN
    {NULL,       Binary,    PREC_EQUALITY},     //      GREATER_TOKEN
    {NULL,       Binary,    PREC_COMPARISON},   //      GREATER_EQUAL_TOKEN
    {NULL,       Binary,    PREC_COMPARISON},   //      LESSER_TOKEN
    {NULL,       Binary,    PREC_COMPARISON},   //      LESSER_EQUAL_TOKEN
    {NULL,       NULL,      PREC_NONE},         //      SEMICOLON_TOKEN
    {NULL,       NULL,      PREC_NONE},         //      COMMA_TOKEN
    {NULL,       NULL,      PREC_NONE},         //      DOT_TOKEN
    {NULL,       Binary,    PREC_TERM},         //      PLUS_TOKEN
    {Unary,      Binary,    PREC_TERM},         //      MINUS_TOKEN
    {NULL,       Binary,    PREC_FACTOR},       //      STAR_TOKEN
    {NULL,       Binary,    PREC_FACTOR},       //      SLASH_TOKEN
    {Number,     NULL,      PREC_NONE},         //      NUMBER_TOKEN
    {String,     NULL,      PREC_NONE},         //      STRING_TOKEN
    {NULL,       NULL,      PREC_NONE},         //      EOF_TOKEN
    {NULL,       NULL,      PREC_NONE},         //      IF_TOKEN
    {NULL,       NULL,      PREC_NONE},         //      ELIF_TOKEN
    {NULL,       NULL,      PREC_NONE},         //      ELSE_TOKEN
    {NULL,       NULL,      PREC_NONE},         //      FOR_TOKEN
    {NULL,       NULL,      PREC_NONE},         //      IN_TOKEN
    {NULL,       NULL,      PREC_NONE},         //      WHILE_TOKEN
    {NULL,       NULL,      PREC_NONE},         //      ERROR_TOKEN
    {NULL,       NULL,      PREC_NONE},         //      LAMBDA_TOKEN
    {NULL,       Binary,    PREC_AND},          //      AND_TOKEN
    {NULL,       Binary,    PREC_OR},           //      OR_TOKEN
    {Unary,      NULL,      PREC_NOT},          //      NOT_TOKEN
    {NULL,       NULL,      PREC_NONE},         //      TRY_TOKEN
    {NULL,       NULL,      PREC_NONE},         //      WITH_TOKEN
    {NULL,       NULL,      PREC_NONE},         //      DEF_TOKEN
    {NULL,       NULL,      PREC_NONE},         //      CLASS_TOKEN
    {NULL,       NULL,      PREC_NONE},         //      COLON_TOKEN
    {NULL,       NULL,      PREC_NONE},         //      COLON_EQUAL_TOKEN
    {NULL,       NULL,      PREC_NONE},         //      INDENT_TOKEN
    {NULL,       NULL,      PREC_NONE},         //      NEWLINE_TOKEN
    {NULL,       NULL,      PREC_NONE},         //      DEDENT_TOKEN
    {NULL,       NULL,      PREC_NONE},         //      TYPE_COMMENT_TOKEN
    {NULL,       NULL,      PREC_NONE},         //      EXCEPT_TOKEN
    {NULL,       NULL,      PREC_NONE},         //      AS_TOKEN
    {NULL,       NULL,      PREC_NONE},         //      NAME_TOKEN
    {NULL,       NULL,      PREC_NONE},         //      PRINT_TOKEN
    {Boolean,    NULL,      PREC_NONE},         //      TRUE_TOKEN
    {Boolean,    NULL,      PREC_NONE},         //      FALSE_TOKEN
    {NULL,       NULL,      PREC_NONE},         //      GLOBAL_TOKEN
    {Identifier, NULL,      PREC_NONE},         //      IDENTIFIER_TOKEN
    {NULL,       Binary,    PREC_POWER}         //      POWER_TOKEN
};

static ParseRule* GetRule(TokenType type) {
    return &rules[type];
}

void ParsePrecedence(Precedence precedence) {
    GetNextToken();
    ParseFn prefixRule = GetRule(parser.previous.type)->prefix;
    
    if (prefixRule == NULL) {
        Error("Expect expression \n");
        return;
    }
    
    bool canAssign = precedence <= PREC_ASSIGNMENT;
    prefixRule(canAssign);
    
    while (precedence <= GetRule(parser.current.type)->precedence) {
        GetNextToken();
        ParseFn infixRule = GetRule(parser.previous.type)->infix;
        infixRule(canAssign);
    }
    
    if (canAssign && MatchToken(EQUAL_TOKEN)) {
        Error("Invalid assignment target");
        Expression();
    }
}

ObjFunction* Compile(Bytecode* bytecode, const char* sourceCode, const char* path) {
    InitScanner(sourceCode, path);
    Compiler compiler;
    InitCompiler(&compiler, TYPE_SCRIPT);
    InitParser();
    
    GetNextToken();
    if (strncmp(path, "<stdin>", 7) == 0) {
        TestList();
    }
    else {
        while (parser.current.type != EOF_TOKEN) {
            if (!MatchToken(NEWLINE_TOKEN) && !MatchToken(EOF_TOKEN)) {
                Statement();
            }
        }
    }
    
    ObjFunction* function = EndCompiler();
    return parser.hadError ? NULL : function;
}
