#ifndef PyInt_debug_h
#define PyInt_debug_h

#include "Bytecode.h"	
#include "InterpreterSettings.h"
#include "Value.h"
#include "Variable.h"

void DisassembleBytecode(Local* local, Bytecode* bytecode, const char* fileName, IOSettings* settings);
void InitialiseExecutionDisassembly(IOSettings* settings);
void DisassembleExecution(Bytecode* bytecode, int offset, Value* stack, Value* stackTop, IOSettings* settings);

#endif
