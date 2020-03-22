#ifndef PyInt_Local_h
#define PyInt_Local_h

#include <stdio.h>

#include "Token.h"
#include "Common.h"

typedef struct {
    int depth;
    Token name;
} Local;

uint8_t IdentifierConstant(Token* name);
int DeclareVariable(void);
uint8_t ParseVariable(void);
int ResolveLocal(Token* name);
void BeginScope(void);
void EndScope(void);
void MarkInitialised(void);
void DefineVariable(uint8_t global);
#endif
