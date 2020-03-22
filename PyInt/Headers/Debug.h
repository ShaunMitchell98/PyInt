#ifndef PyInt_debug_h
#define PyInt_debug_h

#include "Bytecode.h"	

void DisassembleBytecode(Bytecode* bytecode);
void DisassembleExecution(Bytecode* bytecode, int offset, Value* stack, Value* stackTop);

#endif
