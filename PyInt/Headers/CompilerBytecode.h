#ifndef PyInt_CompilerBytecode_h
#define PyInt_CompilerBytecode_h

#include "Compiler.h"

void WriteByte(Compiler* compiler, uint8_t byte);
void WriteBytes(Compiler* compiler, uint8_t byte1, uint8_t byte2);
void WriteReturn(Compiler* compiler);
uint8_t StoreConstant(Compiler* compiler, Value value);
void WriteConstantOperation(Compiler* compiler, Value value);
int WriteJump(Compiler* compiler, uint8_t opcode);
void WriteLoop(Compiler* compiler, int loopStart);
void PatchJump(Compiler* compiler, int offset);

#endif
