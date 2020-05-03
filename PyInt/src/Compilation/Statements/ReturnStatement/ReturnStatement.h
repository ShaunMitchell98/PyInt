#ifndef PyInt_ReturnStatement_h
#define PyInt_ReturnStatement_h

#include "../../Services/Services.h"
#include "../../../Services/Bytecode/Bytecode.h"
#include "../../Compiler/Compiler.h"

void ReturnStatement(Compiler* compiler, Services* services, Bytecode* bytecode);

#endif