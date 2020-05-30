#ifndef PyInt_LocalFunctions_h
#define PyInt_Local_Functions_h

#include "Local.h"
#include "../../../Services/Bytecode/Bytecode.h"
#include "../../Services/Services.h"
#include "../../../Headers/Token.h"
#include "../../Compiler/Compiler.h"

bool IdentifiersEqual(Token* a, Token* b);
int GetLocalStackOffset(Local* locals, int localCount, Token* token);
int PushLocalToCompilerStack(Compiler* compiler, Token* token);
int SetNewLocalVariable(Compiler* compiler, Bytecode* bytecode, Services* services, Token* identifier);
void SetExistingLocalVariable(Compiler* compiler, Bytecode* bytecode, Services* services, Token* identifier, int localStackOffset);

#endif