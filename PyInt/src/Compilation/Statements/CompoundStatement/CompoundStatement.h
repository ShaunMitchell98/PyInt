#ifndef PyInt_CompoundStatement_h
#define PyInt_CompoundStatement_h

#include "../../../Headers/Common.h"
#include "../../Services/Services.h"
#include "../../../Services/Bytecode/Bytecode.h"
#include "../../Compiler/Compiler.h"

bool CompoundStatement(Compiler* compiler, Services* services, Bytecode* bytecode);

#endif