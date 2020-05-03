#ifndef PyInt_LocalFunctions_h
#define PyInt_Local_Functions_h

#include "Local.h"
#include "../../../Services/Bytecode/Bytecode.h"
#include "../../Services/Services.h"
#include "../../../Headers/Token.h"
#include "../../Compiler/Compiler.h"

int GetLocalStackOffset(Local* locals, int localCount, Token* token);
int PushLocalToCompilerStack(Compiler* compiler, Token* token);
void SetLocalVariable(Compiler* compiler, Bytecode* bytecode, Services* services);

#endif