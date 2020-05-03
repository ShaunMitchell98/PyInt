#ifndef PyInt_ExecutionDisassembly_h
#define PyInt_ExecutionDisassembly_h

#include "../../Bytecode/Bytecode.h"
#include "../../../Types/Value/Value.h"
#include "../../../Virtual Machine/Settings/Settings.h"

void InitialiseExecutionDisassembly(IOSettings* executionSettings);
void DisassembleExecution(Bytecode* bytecode, int offset, Value* stack, Value* stackTop, IOSettings* executionSettings);

#endif