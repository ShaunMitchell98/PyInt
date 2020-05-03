#ifndef PyInt_UpvalueResolver_h
#define PyInt_UpvalueResolver_h

#include "../../Compiler/Compiler.h"
#include "../../../Headers/Token.h"

int ResolveUpvalue(Compiler* compiler, Token* token);
Token* GetUpvalueToken(Compiler* compiler, int upvalueIndex, bool isLocal);

#endif