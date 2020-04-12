#ifndef PyInt_debug_h
#define PyInt_debug_h

#include "Bytecode.h"	
#include "InterpreterSettings.h"
#include "VM.h"

void DisassembleBytecode(VM* vm, Bytecode* bytecode, IOSettings* settings);
void InitialiseExecutionDisassembly(IOSettings* settings);
void DisassembleExecution(VM* vm, Bytecode* bytecode, int offset, Value* stack, Value* stackTop, IOSettings* settings);

#endif
