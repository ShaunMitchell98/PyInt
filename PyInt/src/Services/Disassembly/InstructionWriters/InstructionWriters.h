#ifndef PyInt_InstructionWriters_h
#define PyInt_InstructionWriters_h

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "../../../Headers/Common.h"
#include "../../../Types/Value/Value.h"
#include "../../../Virtual Machine/VM/VM.h"

int WriteInstructionWithoutConstant(const char* instructionName, char* buffer, int bufferSize, int offset);
int WriteInstructionWithConstant(IOSettings* settings, const char* instructionName, char* buffer, int bufferSize, uint8_t constant, Value value, int offset);
int WriteClosureOperation(IOSettings* settings, char* buffer, int bufferSize, Bytecode* bytecode, int offset);
int WriteInstructionWithJump(const char* instructionName, char* buffer, int bufferSize, Bytecode* bytecode, int sign, int offset);
void DisassembleStack(IOSettings* settings, Value* stack, Value* stackTop, char* buffer, int bufferSize);
void WriteInstructionAddress(char* buffer, int bufferSize, int offset);
void WriteLineNumber(Bytecode* bytecode, char* buffer, int bufferSize, int offset);
#endif
