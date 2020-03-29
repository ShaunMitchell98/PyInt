#ifndef PyInt_Local_h
#define PyInt_Local_h

#include <stdio.h>

#include "Token.h"
#include "Common.h"

typedef struct sCompiler Compiler;

typedef struct {
    int depth;
    Token name;
} Local;

uint8_t IdentifierConstant(Compiler* compiler, Token* name);
int DeclareVariable(Compiler* compiler);
uint8_t ParseVariable(Compiler* compiler);
int ResolveLocal(Compiler* compiler, Token* name);
void BeginScope(Compiler* compiler);
void EndScope(Compiler* compiler);
void MarkInitialised(Compiler* compiler);
void DefineVariable(Compiler* compiler, uint8_t global);
#endif
