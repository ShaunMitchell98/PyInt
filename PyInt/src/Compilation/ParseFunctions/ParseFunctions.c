#include <string.h>
#include <stdlib.h>

#include "ParseFunctions.h"
#include "../../Headers/Common.h"
#include "../../Types/Value/Value.h"
#include "../../Services/Memory/Memory.h"
#include "../../Services/Bytecode/Bytecode.h"
#include "../Bytecode/CompilerBytecode.h"
#include "../Helpers/Helpers.h"
#include "../Errors/Errors.h"
#include "../../Services/Table/TableFunctions.h"
#include "../../Types/Function/Function.h"
#include "../Variables/Local/LocalFunctions.h"
#include "../Variables/Global/Global.h"
#include "../Variables/Upvalue/UpvalueResolver.h"
#include "../Services/Services.h"
#include "../Compiler/Compiler.h"

static ParseRule* GetRule(TokenType type);

static void NumberToken(Compiler* compiler, Services* services, Bytecode* bytecode, bool canAssign) {
    double number = strtod(services->parser->previous.start, NULL);
    WriteConstantOperation(bytecode, services, NUMBER_VAL(number));
}

static void StringToken(Compiler* compiler, Services* services, Bytecode* bytecode, bool canAssign) {
    char* string = ALLOCATE(services->garbageCollector, char, services->parser->previous.length);
    string = memcpy(string, services->parser->previous.start, services->parser->previous.length);
    uint8_t address = StoreInBytecodeValueArray(bytecode, services, OBJ_VAL(CopyStringToTable(services->garbageCollector, services->stringsTable, services->parser->previous.start, services->parser->previous.length)));
    WriteBytes(bytecode, services, CONSTANT_OP, address);
    GetNextToken(services);
}

static void BooleanToken(Compiler* compiler, Services* services, Bytecode* bytecode, bool canAssign) {
    bool boolean = services->parser->previous.type == TRUE_TOKEN;
    WriteConstantOperation(bytecode, services, BOOLEAN_VAL(boolean));
}

static void GroupingToken(Compiler* compiler, Services* services, Bytecode* bytecode, bool canAssign) {
    ParsePrecedence(compiler, services, bytecode, PREC_ASSIGNMENT);
    ConsumeToken(services, RIGHT_PAREN_TOKEN, RightParenError);
}

static void BinaryToken(Compiler* compiler, Services* services, Bytecode* bytecode, bool canAssign) {
    TokenType operatorType = services->parser->previous.type;

    //Compile the right operand
    ParseRule* rule = GetRule(operatorType);
    ParsePrecedence(compiler, services, bytecode, (Precedence)rule->precedence + 1);

    switch (operatorType) {
    case NOT_EQUAL_TOKEN: WriteBytes(bytecode, services, EQUAL_OP, NOT_OP); break;
    case EQUAL_EQUAL_TOKEN: WriteByte(bytecode, services, EQUAL_OP); break;
    case GREATER_TOKEN: WriteByte(bytecode, services, GREATER_OP); break;
    case GREATER_EQUAL_TOKEN: WriteBytes(bytecode, services, LESSER_OP, NOT_OP); break;
    case LESSER_TOKEN: WriteByte(bytecode, services, LESSER_OP); break;
    case LESSER_EQUAL_TOKEN: WriteBytes(bytecode, services, GREATER_OP, NOT_OP); break;
    case PLUS_TOKEN: WriteByte(bytecode, services, ADD_OP); break;
    case MINUS_TOKEN: WriteByte(bytecode, services, SUBTRACT_OP); break;
    case STAR_TOKEN: WriteByte(bytecode, services, MULTIPLY_OP); break;
    case SLASH_TOKEN: WriteByte(bytecode, services, DIVIDE_OP); break;
    case POWER_TOKEN: WriteByte(bytecode, services, POWER_OP); break;
    case AND_TOKEN: WriteByte(bytecode, services, AND_OP); break;
    case OR_TOKEN: WriteByte(bytecode, services, OR_OP); break;
    case NOT_TOKEN: WriteByte(bytecode, services, NOT_OP); break;
    default: return;
    }
}

static void UnaryToken(Compiler* compiler, Services* services, Bytecode* bytecode, bool canAssign) {
    TokenType operatorType = services->parser->previous.type;

    ParsePrecedence(compiler, services, bytecode, PREC_UNARY);

    switch (operatorType) {
    case NOT_TOKEN: WriteByte(bytecode, services, NOT_OP); break;
    case MINUS_TOKEN: WriteByte(bytecode, services, NEGATE_OP); break;
    default: return;
    }
}

static int NewLocalIdentifier(Compiler* compiler, Services* services, Bytecode* bytecode, Token* identifier) {
    if (MatchToken(services, services->parser->current, EQUAL_TOKEN)) {
        ParsePrecedence(compiler, services, bytecode, PREC_ASSIGNMENT);
        return SetNewLocalVariable(compiler, bytecode, services, identifier);
    }
    else {
        return -1;
    }
}

static int ExistingLocalIdentifier(Compiler* compiler, Services* services, Bytecode* bytecode, Token* identifier, int localStackOffset) {
    if (MatchToken(services, services->parser->current, EQUAL_TOKEN)) {
        ParsePrecedence(compiler, services, bytecode, PREC_ASSIGNMENT);
        SetExistingLocalVariable(compiler, bytecode, services, identifier, localStackOffset);
    }
    else {
        WriteBytes(bytecode, services, GET_LOCAL_OP, (uint8_t)localStackOffset);
    }
    return localStackOffset;
}

static int LocalIdentifier(Compiler* compiler, Services* services, Bytecode* bytecode) {
    Token* identifier = (Token*)malloc(sizeof(Token));
    *identifier = services->parser->previous;
    int localStackOffset = GetLocalStackOffset(compiler->locals, compiler->localCount, identifier);

    if (localStackOffset == -1) {
        localStackOffset = NewLocalIdentifier(compiler, services, bytecode, identifier);
    }
    else {
        localStackOffset = ExistingLocalIdentifier(compiler, services, bytecode, identifier, localStackOffset);
    }

    free(identifier);
    return localStackOffset;
}

static int UpvalueIdentifier(Compiler* compiler, Services* services, Bytecode* bytecode) {
    Token identifier = services->parser->previous;
    int upvalueIndex = ResolveUpvalue(compiler, services, &identifier);

    if (upvalueIndex != -1) {
        if (MatchToken(services, services->parser->current, EQUAL_TOKEN)) {
            ParsePrecedence(compiler, services, bytecode, PREC_ASSIGNMENT);
            SetNewLocalVariable(compiler, bytecode, services, &identifier);
        }
        else {
            WriteBytes(bytecode, services, GET_UPVALUE_OP, (uint8_t)upvalueIndex);
            return upvalueIndex;
        }
    }
    
    return upvalueIndex;
}

static void GlobalIdentifier(Compiler* compiler, Services* services, Bytecode* bytecode, bool canAssign) {
    int arg = StoreGlobalInBytecodeConstantTable(bytecode, services, &services->parser->previous);

    if (canAssign && MatchToken(services, services->parser->current, EQUAL_TOKEN)) {
        ParsePrecedence(compiler, services, bytecode, PREC_ASSIGNMENT);
        WriteBytes(bytecode, services, SET_GLOBAL_OP, (uint8_t)arg);
    }
    else {
        WriteBytes(bytecode, services, GET_GLOBAL_OP, (uint8_t)arg);
    }
}

static void IdentifierToken(Compiler* compiler, Services* services, Bytecode* bytecode, bool canAssign) {

    if (compiler->enclosing != NULL) {
        int localStackOffset = LocalIdentifier(compiler, services, bytecode);
        if (localStackOffset == -1) {
            int upvalueIndex = UpvalueIdentifier(compiler, services, bytecode);
            if (upvalueIndex == -1) {
                GlobalIdentifier(compiler, services, bytecode, canAssign);

            }
        }
    }
    else {
        GlobalIdentifier(compiler, services, bytecode, canAssign);
    }
}

static uint8_t ArguementList(Compiler* compiler, Services* services, Bytecode* bytecode) {
    uint8_t argCount = 0;
    if (!CheckToken(services, RIGHT_PAREN_TOKEN)) {
        do {
            ParsePrecedence(compiler, services, bytecode, PREC_ASSIGNMENT);

            if (argCount == 255) {
                Error("Cannot have more than 255 arguements");
            }

            argCount++;
        } while (MatchToken(services, services->parser->current, COMMA_TOKEN));
    }

    ConsumeToken(services, RIGHT_PAREN_TOKEN, "Expect ')' after arguements");
    return argCount;
}

static void CallToken(Compiler* compiler, Services* services, Bytecode* bytecode, bool canAssign) {
    uint8_t argCount = ArguementList(compiler, services, bytecode);
    WriteBytes(bytecode, services, CALL_OP, argCount);
}

static void DotToken(Compiler* compiler, Services* services, Bytecode* bytecode, bool canAssign) {
    ConsumeToken(services, IDENTIFIER_TOKEN, DotCallError);
    uint8_t address = StoreInBytecodeValueArray(bytecode, services, OBJ_VAL(CopyStringToTable(services->garbageCollector, services->stringsTable, services->parser->previous.start, services->parser->previous.length)));

    if (canAssign && MatchToken(services, services->parser->current, EQUAL_TOKEN)) {
        ParsePrecedence(compiler, services, bytecode, PREC_ASSIGNMENT);
        WriteBytes(bytecode, services, SET_PROPERTY_OP, address);
    }
    else {
        WriteBytes(bytecode, services, GET_PROPERTY_OP, address);
    }
}

ParseRule rules[] = {
    {GroupingToken,   CallToken,      PREC_CALL},         //      LEFT_PAREN_TOKEN
    {NULL,            NULL,           PREC_NONE},         //      RIGHT_PAREN_TOKEN
    {NULL,            NULL,           PREC_NONE},         //      LEFT_BRACE_TOKEN
    {NULL,            NULL,           PREC_NONE},         //      RIGHT_BRACE_TOKEN
    {NULL,            NULL,           PREC_NONE},         //      EQUAL_TOKEN
    {NULL,            BinaryToken,    PREC_EQUALITY},     //      BANG_EQUAL_TOKEN
    {NULL,            BinaryToken,    PREC_EQUALITY},     //      EQUAL_EQUAL_TOKEN
    {NULL,            BinaryToken,    PREC_EQUALITY},     //      GREATER_TOKEN
    {NULL,            BinaryToken,    PREC_COMPARISON},   //      GREATER_EQUAL_TOKEN
    {NULL,            BinaryToken,    PREC_COMPARISON},   //      LESSER_TOKEN
    {NULL,            BinaryToken,    PREC_COMPARISON},   //      LESSER_EQUAL_TOKEN
    {NULL,            NULL,           PREC_NONE},         //      SEMICOLON_TOKEN
    {NULL,            NULL,           PREC_NONE},         //      COMMA_TOKEN
    {NULL,            DotToken,       PREC_CALL},         //      DOT_TOKEN
    {NULL,            BinaryToken,    PREC_TERM},         //      PLUS_TOKEN
    {UnaryToken,      BinaryToken,    PREC_TERM},         //      MINUS_TOKEN
    {NULL,            BinaryToken,    PREC_FACTOR},       //      STAR_TOKEN
    {NULL,            BinaryToken,    PREC_FACTOR},       //      SLASH_TOKEN
    {NumberToken,     NULL,           PREC_NONE},         //      NUMBER_TOKEN
    {StringToken,     NULL,           PREC_NONE},         //      STRING_TOKEN
    {NULL,            NULL,           PREC_NONE},         //      EOF_TOKEN
    {NULL,            NULL,           PREC_NONE},         //      IF_TOKEN
    {NULL,            NULL,           PREC_NONE},         //      ELIF_TOKEN
    {NULL,            NULL,           PREC_NONE},         //      ELSE_TOKEN
    {NULL,            NULL,           PREC_NONE},         //      FOR_TOKEN
    {NULL,            NULL,           PREC_NONE},         //      IN_TOKEN
    {NULL,            NULL,           PREC_NONE},         //      WHILE_TOKEN
    {NULL,            NULL,           PREC_NONE},         //      ERROR_TOKEN
    {NULL,            NULL,           PREC_NONE},         //      LAMBDA_TOKEN
    {NULL,            BinaryToken,    PREC_AND},          //      AND_TOKEN
    {NULL,            BinaryToken,    PREC_OR},           //      OR_TOKEN
    {UnaryToken,      NULL,           PREC_NOT},          //      NOT_TOKEN
    {NULL,            NULL,           PREC_NONE},         //      TRY_TOKEN
    {NULL,            NULL,           PREC_NONE},         //      WITH_TOKEN
    {NULL,            NULL,           PREC_NONE},         //      DEF_TOKEN
    {NULL,            NULL,           PREC_NONE},         //      CLASS_TOKEN
    {NULL,            NULL,           PREC_NONE},         //      COLON_TOKEN
    {NULL,            NULL,           PREC_NONE},         //      COLON_EQUAL_TOKEN
    {NULL,            NULL,           PREC_NONE},         //      INDENT_TOKEN
    {NULL,            NULL,           PREC_NONE},         //      NEWLINE_TOKEN
    {NULL,            NULL,           PREC_NONE},         //      DEDENT_TOKEN
    {NULL,            NULL,           PREC_NONE},         //      TYPE_COMMENT_TOKEN
    {NULL,            NULL,           PREC_NONE},         //      EXCEPT_TOKEN
    {NULL,            NULL,           PREC_NONE},         //      AS_TOKEN
    {NULL,            NULL,           PREC_NONE},         //      NAME_TOKEN
    {BooleanToken,    NULL,           PREC_NONE},         //      TRUE_TOKEN
    {BooleanToken,    NULL,           PREC_NONE},         //      FALSE_TOKEN
    {NULL,            NULL,           PREC_NONE},         //      GLOBAL_TOKEN
    {IdentifierToken, NULL,           PREC_NONE},         //      IDENTIFIER_TOKEN
    {NULL,            BinaryToken,    PREC_POWER},        //      POWER_TOKEN
    {NULL,            NULL,           PREC_NONE},         //      RETURN_TOKEN
};

static ParseRule* GetRule(TokenType type) {
    return &rules[type];
}

void ParsePrecedence(Compiler* compiler, Services* services, Bytecode* bytecode, Precedence precedence) {
    GetNextToken(services);
    ParseFn prefixRule = GetRule(services->parser->previous.type)->prefix;

    if (prefixRule == NULL) {
        Error("Expect expression \n");
        return;
    }

    bool canAssign = precedence <= PREC_ASSIGNMENT;
    prefixRule(compiler, services, bytecode, canAssign);

    while (precedence <= GetRule(services->parser->current.type)->precedence) {
        GetNextToken(services);
        ParseFn infixRule = GetRule(services->parser->previous.type)->infix;
        infixRule(compiler, services, bytecode, canAssign);
    }

    if (canAssign && MatchToken(services, services->parser->current, EQUAL_TOKEN)) {
        Error("Invalid assignment target");
        ParsePrecedence(compiler, services, bytecode, PREC_ASSIGNMENT);
    }
}