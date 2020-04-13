#ifndef PyInt_Variable_h
#define PyInt_Variable_h

#include <stdio.h>

#include "Token.h"
#include "Common.h"

typedef struct sCompiler Compiler;

typedef struct {
    int depth;
    Token name;
} Local;

uint8_t StoreGlobalInBytecodeConstants(Compiler* compiler, Token* name);
int PushLocalToCompilerStack(Compiler* compiler);
int GetLocalStackOffset(Compiler* compiler, Token* name);
void SetLocalVariable(Compiler* compiler);
void SetGlobalVariable(Compiler* compiler);
void BeginScope(Compiler* compiler);
void EndScope(Compiler* compiler);
uint8_t ArguementList(Compiler* compiler);
#endif
