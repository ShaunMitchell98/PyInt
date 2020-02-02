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
void DeclareVariable(void);
uint8_t ParseVariable(const char* errorMessage);
int ResolveLocal(Token* name);
#endif
