#ifndef PyInt_Global_h
#define PyInt_Global_h

#include "../../../Headers/Common.h"
#include "../../../Services/Bytecode/Bytecode.h"
#include "../../Services/Services.h"
#include "../../../Headers/Token.h"

uint8_t StoreGlobalInBytecodeConstantTable(Bytecode* bytecode, Services* services, Token* name);
void SetGlobalVariable(Bytecode* bytecode, Services* services);

#endif