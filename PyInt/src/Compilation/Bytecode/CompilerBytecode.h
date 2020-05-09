#ifndef PyInt_CompilerBytecode_h
#define PyInt_CompilerBytecode_h

#include "../../Services/Bytecode/Bytecode.h"
#include "../../Headers/Common.h"
#include "../Services/Services.h"
#include "../../Types/Value/Value.h"

void WriteByte(Bytecode* bytecode, Services* services, uint8_t byte);
void WriteBytes(Bytecode* bytecode, Services* services, uint8_t byte1, uint8_t byte2);
void WriteReturn(Bytecode* bytecode, Services* services);
uint8_t StoreInBytecodeValueArray(Bytecode* bytecode, Services* services, Value value);
void WriteConstantOperation(Bytecode* bytecode, Services* services, Value value);
int WriteJump(Bytecode* bytecode, Services* services, uint8_t opcode);
void WriteLoop(Bytecode* bytecode, Services* services, int loopStart);
void PatchJump(Bytecode* bytecode, int offset);

#endif
