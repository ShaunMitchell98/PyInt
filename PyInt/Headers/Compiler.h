#ifndef PyInt_Compiler_h
#define PyInt_Compiler_h

#include "Bytecode.h"
#include "Token.h"
#include "Scanner.h"
#include "Object.h"
#include "Local.h"
#include "VM.h"

typedef struct {
    Token previous;
    Token current;
    bool hadError;
} Parser;

struct sCompiler {
    struct sCompiler* enclosing;
    ObjFunction* function;
    FunctionType functionType;
    Local locals[UINT8_COUNT];
    int localCount;
    int scopeDepth;
    Scanner* scanner;
    Parser* parser;
    VM* vm;
};

typedef enum {
    PREC_NONE,
    PREC_ASSIGNMENT,
    PREC_OR,
    PREC_AND,
    PREC_NOT,
    PREC_EQUALITY,
    PREC_COMPARISON,
    PREC_TERM,
    PREC_FACTOR,
    PREC_POWER,
    PREC_UNARY,
    PREC_CALL,
    PREC_PRIMARY
} Precedence;

typedef void (*ParseFn)(Compiler* compiler, bool canAssign);

typedef struct {
    ParseFn prefix;
    ParseFn infix;
    Precedence precedence;
} ParseRule;

ObjFunction* Compile(VM* vm, Bytecode* bytecode, const char* sourceCode, const char* path);
void ParsePrecedence(Compiler* compiler, Precedence precedence);

#endif
