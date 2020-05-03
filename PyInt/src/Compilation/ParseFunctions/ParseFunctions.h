#ifndef PyInt_ParseFunctions_h
#define PyInt_ParseFunctions_h

#include "../Services/Services.h"
#include "../../Services/Bytecode/Bytecode.h"
#include "../../Headers/Common.h"
#include "../Compiler/Compiler.h"

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

typedef void (*ParseFn)(Compiler* compiler, Services* services, Bytecode* bytecode, bool canAssign);

typedef struct {
    ParseFn prefix;
    ParseFn infix;
    Precedence precedence;
} ParseRule;

void ParsePrecedence(Compiler* compiler, Services* services, Bytecode* bytecode, Precedence precedence);

#endif
