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

static void InitParser(Parser* parser) {
    parser->hadError = false;
}

static void InitCompiler(Compiler* currentCompiler, Compiler* compiler, FunctionType functionType) {
    compiler->function = NULL;
    compiler->functionType = functionType;
    compiler->localCount = 0;
    compiler->scopeDepth = 0;
    compiler->function = NewFunction();
    compiler->enclosing = (struct Compiler*) currentCompiler;
    
    if (functionType != TYPE_SCRIPT) {
        compiler->function->name = CopyString(currentCompiler->parser->previous.start, currentCompiler->parser->previous.length);
        compiler->parser = currentCompiler->parser;
    }

    Local* local = &compiler->locals[compiler->localCount++];
    local->depth = 0;
    local->name.start = "";
    local->name.length = 0;
}

static ObjFunction* EndCompiler(Compiler* compiler) {
    WriteReturn(compiler);

    if (compiler->enclosing != NULL) {
        compiler = compiler->enclosing;
    }
#ifdef DEBUG_PRINT_CODE
    if (!parser.hadError) {
        DisassembleBytecode(compiler.function.bytecode, compiler.function->name != NULL ? compiler.function->name->chars : "<script>");
    }
#endif
    return compiler->function;
}

static ParseRule* GetRule(TokenType type);


static void GlobalStatement(Compiler* compiler) {
    do {
        uint8_t address = StoreConstant(compiler, OBJ_VAL(CopyString(compiler->parser->current.start, compiler->parser->current.length)));
        WriteBytes(compiler, DEFINE_GLOBAL_OP, address);
        GetNextToken(compiler);
    } while (MatchToken(compiler, compiler->parser->current, COMMA_TOKEN));
}

static void Expression(Compiler* compiler) {
    ParsePrecedence(compiler, PREC_ASSIGNMENT);
}

static void StarExpression(Compiler* compiler) {
    Expression(compiler);
}

static void LambdaDef() {
    
}

static void Test(Compiler* compiler) {
    if (MatchToken(compiler, compiler->parser->current, LAMBDA_TOKEN)) {
        LambdaDef();
    } else {
        ParsePrecedence(compiler, PREC_ASSIGNMENT);
    }
    if (MatchToken(compiler, compiler->parser->current, IF_TOKEN)) {
        ParsePrecedence(compiler, PREC_OR);
        if (MatchToken(compiler, compiler->parser->current, IF_TOKEN)) {
            Test(compiler);
        }
    }
}

static void TestListStarExpression(Compiler* compiler) {
    do {
        if (MatchToken(compiler, compiler->parser->current, STAR_TOKEN)) {
            StarExpression(compiler);
        }
        else {
            Test(compiler);
        }
    }
    while (MatchToken(compiler, compiler->parser->current, COMMA_TOKEN));
}

static void ExpressionStatement(Compiler* compiler) {
    TestListStarExpression(compiler);
    
    if(MatchToken(compiler, compiler->parser->current, COLON_TOKEN)) {
        Test(compiler);
    }
    if(MatchToken(compiler, compiler->parser->current, EQUAL_TOKEN)) {
        ParsePrecedence(compiler, PREC_ASSIGNMENT);
    }
}

static void SmallStatement(Compiler* compiler) {
  
    //DelStatement();
    //PassStatement();
    //FlowStatement();
    //ImportStatement();
    if (MatchToken(compiler, compiler->parser->current, GLOBAL_TOKEN)) {
        GlobalStatement(compiler);
    }
    else {
        ExpressionStatement(compiler);
    }
    //NonlocalStatement();
    //AssertStatement();
}

static void SimpleStatement(Compiler* compiler) {
    do {
        SmallStatement(compiler);
    } while (MatchToken(compiler, compiler->parser->current, SEMICOLON_TOKEN));
}

static void Comparison(Compiler* compiler) {
    Expression(compiler);
}

static void TestList(Compiler* compiler) {
    do {
        Test(compiler);
    } while (MatchToken(compiler, compiler->parser->current, COMMA_TOKEN));
}

static void NamedExpressionTest(Compiler* compiler) {
    Test(compiler);
    if (MatchToken(compiler, compiler->parser->current, COLON_EQUAL_TOKEN)) {
        Test(compiler);
    }
}

static void Statement(Compiler* compiler);

static void Suite(Compiler* compiler) {
    if (MatchToken(compiler, compiler->parser->current, NEWLINE_TOKEN)) {
        ConsumeToken(compiler, INDENT_TOKEN, IndentError);
        do {
            Statement(compiler);
            TryConsumeToken(compiler, NEWLINE_TOKEN);
        } while(!MatchToken(compiler, compiler->parser->current, DEDENT_TOKEN) && !MatchToken(compiler, compiler->parser->current, EOF_TOKEN));
    }
    else {
        SimpleStatement(compiler);
    }
}

static void ExpressionList(Compiler* compiler) {
    do {
        if (MatchToken(compiler, compiler->parser->current, STAR_TOKEN)) {
            StarExpression(compiler);
        }
        else {
            Expression(compiler);
        }
    } while (MatchToken(compiler, compiler->parser->current, COMMA_TOKEN));
}

static void ExceptClause(Compiler* compiler) {
    Test(compiler);
    if (MatchToken(compiler, compiler->parser->current, AS_TOKEN)) {
        ConsumeToken(compiler, NAME_TOKEN, NameError);
    }
}

static void VariableDeclaration(Compiler* compiler) {
    uint8_t global = ParseVariable(compiler);

    if (MatchToken(compiler, compiler->parser->current, EQUAL_TOKEN)) {
        Expression(compiler);
    }
    else {
        WriteByte(compiler, NONE_OP);
    }

    DefineVariable(compiler, global);
}

static uint8_t GetIdentifierAddress(Compiler* compiler) {
    if (compiler->scopeDepth > 0) {
        return (uint8_t) ResolveLocal(compiler, &compiler->parser->previous);
    }
    else {
        return IdentifierConstant(compiler, &compiler->parser->previous);
    }
}

static void IfStatement(Compiler* compiler) {
    NamedExpressionTest(compiler);
    ConsumeToken(compiler, COLON_TOKEN, ColonError);
    
    int ifFalseOpcodeAddress = WriteJump(compiler, JUMP_IF_FALSE_OP);
    int elifTrueOpcodeAddress = 0;
    int elifFalseOpcodeAddress = 0;
    
    Suite(compiler);
    
    int ifTrueOpcodeAddress = WriteJump(compiler, JUMP_OP);
    PatchJump(compiler, ifFalseOpcodeAddress);
    
    while (MatchToken(compiler, compiler->parser->current, ELIF_TOKEN)) {
        NamedExpressionTest(compiler);
        ConsumeToken(compiler, COLON_TOKEN, ColonError);
        elifFalseOpcodeAddress = WriteJump(compiler, JUMP_IF_FALSE_OP);
        Suite(compiler);
        
        elifTrueOpcodeAddress = WriteJump(compiler, JUMP_OP);
        PatchJump(compiler, elifFalseOpcodeAddress);
    }
    
    if (MatchToken(compiler, compiler->parser->current, ELSE_TOKEN)) {
        ConsumeToken(compiler, COLON_TOKEN, ColonError);
        Suite(compiler);
    }
    
    PatchJump(compiler, ifTrueOpcodeAddress);
    if (elifTrueOpcodeAddress != 0) {
        PatchJump(compiler, elifTrueOpcodeAddress);
    }
}

static void WhileStatement(Compiler* compiler) {
    int loopStart = compiler->function->bytecode.count;
    
    NamedExpressionTest(compiler);
    ConsumeToken(compiler, COLON_TOKEN, ColonError);
    
    int ifFalseOpcodeAddress = WriteJump(compiler, JUMP_IF_FALSE_OP);
    
    Suite(compiler);
    WriteLoop(compiler, loopStart);
    PatchJump(compiler, ifFalseOpcodeAddress);
}

static void ForStatement(Compiler* compiler) {
    GetNextToken(compiler);
    Token loopVariable = compiler->parser->previous;
    VariableDeclaration(compiler);
    ConsumeToken(compiler, IN_TOKEN, InError);
    GetNextToken(compiler);
    uint8_t identifierAddress = GetIdentifierAddress(compiler);
    ConsumeToken(compiler, COLON_TOKEN, ColonError);
    
    int loopStart = compiler->function->bytecode.count;
    WriteBytes(compiler, END_OF_ARRAY_OP, identifierAddress);
    
    int ifTrueOpcodeAddress = WriteJump(compiler, JUMP_IF_TRUE_OP);
    WriteBytes(compiler, GET_INDEX_OP, identifierAddress);
    if (compiler->scopeDepth > 0) {
        WriteBytes(compiler, SET_LOCAL_OP, (uint8_t) ResolveLocal(compiler, &loopVariable));
    }
    else {
        WriteBytes(compiler, SET_GLOBAL_OP, IdentifierConstant(compiler, &loopVariable));
    }
    Suite(compiler);
    WriteLoop(compiler, loopStart);
    PatchJump(compiler, ifTrueOpcodeAddress);
}

static void TryStatement(Compiler* compiler) {
    ConsumeToken(compiler, COLON_TOKEN, ColonError);
    Suite(compiler);
    do {
        ConsumeToken(compiler, EXCEPT_TOKEN, ExceptError);
        ExceptClause(compiler);
        ConsumeToken(compiler, COLON_TOKEN, ColonError);
        Suite(compiler);
    } while (MatchToken(compiler, compiler->parser->current, EXCEPT_TOKEN));
}

static void WithStatement() {
    
}

static void Function(Compiler* compiler, FunctionType functionType) {
    Compiler newCompiler;
    InitCompiler(compiler, &newCompiler, TYPE_FUNCTION);
    BeginScope(compiler);
    
    ConsumeToken(compiler, LEFT_PAREN_TOKEN, LeftParenError);
    if (!CheckToken(compiler, RIGHT_PAREN_TOKEN)) {
        do {
            newCompiler.function->arity++;
            if (newCompiler.function->arity > 255) {
                Error("Cannot have more than 255 parameters");
            }
            
            uint8_t paramConstant = ParseVariable(compiler);
            DefineVariable(compiler, paramConstant);
        } while(MatchToken(compiler, compiler->parser->current, COMMA_TOKEN));
    }
    ConsumeToken(compiler, RIGHT_PAREN_TOKEN, RightParenError);
    
    ConsumeToken(compiler, COLON_TOKEN, ColonError);
    Suite(&newCompiler);
    
    ObjFunction* function = EndCompiler(&newCompiler);
    WriteBytes(compiler, CONSTANT_OP, StoreConstant(compiler, OBJ_VAL(function)));
}

static void FunctionDefinition(Compiler* compiler) {
    uint8_t global = ParseVariable(compiler);
    MarkInitialised(compiler);
    Function(compiler, TYPE_FUNCTION);
    DefineVariable(compiler, global);
}

static void ClassDefinition() {
    
}

static void PrintStatement(Compiler* compiler) {
    NamedExpressionTest(compiler);
    WriteByte(compiler, PRINT_OP);
}

static bool CompoundStatement(Compiler* compiler) {
    if (MatchToken(compiler, compiler->parser->current, IF_TOKEN)) {
        IfStatement(compiler);
        return true;
    }
    else if (MatchToken(compiler, compiler->parser->current, WHILE_TOKEN)) {
        WhileStatement(compiler);
        return true;
    }
    else if (MatchToken(compiler, compiler->parser->current, FOR_TOKEN)) {
        ForStatement(compiler);
        return true;
    }
    else if (MatchToken(compiler, compiler->parser->current, TRY_TOKEN)) {
        TryStatement(compiler);
        return true;
    }
    else if (MatchToken(compiler, compiler->parser->current, WITH_TOKEN)) {
        WithStatement();
        return true;
    }
    else if (MatchToken(compiler, compiler->parser->current, DEF_TOKEN)) {
        FunctionDefinition(compiler);
        return true;
    }
    else if (MatchToken(compiler, compiler->parser->current, CLASS_TOKEN)) {
        ClassDefinition();
        return true;
    }
    else if (MatchToken(compiler, compiler->parser->current, PRINT_TOKEN)) {
        PrintStatement(compiler);
        return true;
    }
    else {
        return false;
    }
}

static void Statement(Compiler* compiler) {
    if (compiler->parser->current.type == NEWLINE_TOKEN) return;
    bool valid = CompoundStatement(compiler);
    if (!valid) {
        SimpleStatement(compiler);
    }
}

static void Unary(Compiler* compiler, bool canAssign) {
    TokenType operatorType = compiler->parser->previous.type;
    
    ParsePrecedence(compiler, PREC_UNARY);
    
    switch(operatorType) {
        case NOT_TOKEN: WriteByte(compiler, NOT_OP); break;
        case MINUS_TOKEN: WriteByte(compiler, NEGATE_OP); break;
        default: return;
    }
}

static void Binary(Compiler* compiler, bool canAssign) {
    TokenType operatorType = compiler->parser->previous.type;
    
    //Compile the right operand
    ParseRule* rule = GetRule(operatorType);
    ParsePrecedence(compiler, (Precedence)rule->precedence+1);
    
    switch(operatorType) {
        case NOT_EQUAL_TOKEN: WriteBytes(compiler, EQUAL_OP, NOT_OP); break;
        case EQUAL_EQUAL_TOKEN: WriteByte(compiler, EQUAL_OP); break;
        case GREATER_TOKEN: WriteByte(compiler, GREATER_OP); break;
        case GREATER_EQUAL_TOKEN: WriteBytes(compiler, LESSER_OP, NOT_OP); break;
        case LESSER_TOKEN: WriteByte(compiler, LESSER_OP); break;
        case LESSER_EQUAL_TOKEN: WriteBytes(compiler, GREATER_OP, NOT_OP); break;
        case PLUS_TOKEN: WriteByte(compiler, ADD_OP); break;
        case MINUS_TOKEN: WriteByte(compiler, SUBTRACT_OP); break;
        case STAR_TOKEN: WriteByte(compiler, MULTIPLY_OP); break;
        case SLASH_TOKEN: WriteByte(compiler, DIVIDE_OP); break;
        case POWER_TOKEN: WriteByte(compiler, POWER_OP); break;
        case AND_TOKEN: WriteByte(compiler, AND_OP); break;
        case OR_TOKEN: WriteByte(compiler, OR_OP); break;
        case NOT_TOKEN: WriteByte(compiler, NOT_OP); break;
        default: return;
    }
    
}

static void Grouping(Compiler* compiler, bool canAssign) {
    Expression(compiler);
    ConsumeToken(compiler, RIGHT_PAREN_TOKEN, RightParenError);
}

static void Number(Compiler* compiler, bool canAssign) {
    double number = strtod(compiler->parser->previous.start, NULL);
    WriteConstantOperation(compiler, NUMBER_VAL(number));
}

static void String(Compiler* compiler, bool canAssign) {
    char* string = ALLOCATE(char, compiler->parser->previous.length);
    string = memcpy(string, compiler->parser->previous.start, compiler->parser->previous.length);
    string[compiler->parser->previous.length] = '\0';
    uint8_t address = StoreConstant(compiler, OBJ_VAL(CopyString(compiler->parser->previous.start, compiler->parser->previous.length+1)));
    WriteBytes(compiler, CONSTANT_OP, address);
    GetNextToken(compiler);
}

static void Boolean(Compiler* compiler, bool canAssign) {
    bool boolean = compiler->parser->previous.start;
    WriteConstantOperation(compiler, BOOLEAN_VAL(boolean));
}

static void LocalIdentifier(Compiler* compiler, bool canAssign) {
    int arg = ResolveLocal(compiler, &compiler->parser->previous);
    
    if (arg == -1) {
        VariableDeclaration(compiler);
    }
    else {
        WriteBytes(compiler, GET_LOCAL_OP, (uint8_t)arg);
    }
}

static void GlobalIdentifier(Compiler* compiler, bool canAssign) {
    int arg = IdentifierConstant(compiler, &compiler->parser->previous);
    
     if (canAssign && MatchToken(compiler, compiler->parser->current, EQUAL_TOKEN)) {
           Expression(compiler);
           WriteBytes(compiler, SET_GLOBAL_OP, (uint8_t)arg);
       }
       else {
           WriteBytes(compiler, GET_GLOBAL_OP, (uint8_t)arg);
       }
}

static void Identifier(Compiler* compiler, bool canAssign) {

    if (compiler->scopeDepth > 0) {
        LocalIdentifier(compiler, canAssign);
    }
    else {
        GlobalIdentifier(compiler, canAssign);
    }
}

ParseRule rules[] = {
    {Grouping,   NULL,      PREC_CALL},         //      LEFT_PAREN_TOKEN
    {NULL,       NULL,      PREC_NONE},         //      RIGHT_PAREN_TOKEN
    {NULL,       NULL,      PREC_NONE},         //      LEFT_BRACE_TOKEN
    {NULL,       NULL,      PREC_NONE},         //      RIGHT_BRACE_TOKEN
    {NULL,       NULL,      PREC_NONE},         //      EQUAL_TOKEN
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

void ParsePrecedence(Compiler* compiler, Precedence precedence) {
    GetNextToken(compiler);
    ParseFn prefixRule = GetRule(compiler->parser->previous.type)->prefix;
    
    if (prefixRule == NULL) {
        Error("Expect expression \n");
        return;
    }
    
    bool canAssign = precedence <= PREC_ASSIGNMENT;
    prefixRule(compiler, canAssign);
    
    while (precedence <= GetRule(compiler->parser->current.type)->precedence) {
        GetNextToken(compiler);
        ParseFn infixRule = GetRule(compiler->parser->previous.type)->infix;
        infixRule(compiler, canAssign);
    }
    
    if (canAssign && MatchToken(compiler, compiler->parser->current, EQUAL_TOKEN)) {
        Error("Invalid assignment target");
        Expression(compiler);
    }
}

ObjFunction* Compile(Bytecode* bytecode, const char* sourceCode, const char* path) {
    Scanner scanner;
    InitScanner(&scanner, sourceCode, path);

    Parser parser;
    InitParser(&parser);

    Compiler compiler;
    compiler.parser = &parser;
    InitCompiler(NULL, &compiler, TYPE_SCRIPT);
    
    compiler.scanner = &scanner;

    GetNextToken(&compiler);
    if (strncmp(path, "<stdin>", 7) == 0) {
        TestList(&compiler);
    }
    else {
        while (compiler.parser->current.type != EOF_TOKEN) {
            if (!MatchToken(&compiler, compiler.parser->current, NEWLINE_TOKEN) && !MatchToken(&compiler, compiler.parser->current, EOF_TOKEN)) {
                Statement(&compiler);
            }
        }
    }
    
    ObjFunction* function = EndCompiler(&compiler);
    return compiler.parser->hadError ? NULL : function;
}
