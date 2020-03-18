#ifndef PyInt_Compiler_h
#define PyInt_Compiler_h

#include "Local.h"
#include "Bytecode.h"
#include "Token.h"
#include "Scanner.h"
#include "Object.h"

#define UINT8_COUNT 256
struct Local;

typedef struct {
    struct Compiler* enclosing;
    ObjFunction* function;
    FunctionType functionType;
    Local locals[UINT8_COUNT];
    int localCount;
    int scopeDepth;
} Compiler;

typedef struct {
    Token previous;
    Token current;
    bool hadError;
} Parser;

Compiler* currentCompiler;
Parser parser;

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

typedef void (*ParseFn)(bool canAssign);

typedef struct {
    ParseFn prefix;
    ParseFn infix;
    Precedence precedence;
} ParseRule;

ObjFunction* Compile(Bytecode* bytecode, const char* sourceCode, const char* path);
void ParsePrecedence(Precedence precedence);

#endif
