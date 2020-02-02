#ifndef PyInt_CompilerBytecode_h
#define PyInt_CompilerBytecode_h

void WriteByte(uint8_t byte);
void WriteBytes(uint8_t byte1, uint8_t byte2);
void WriteReturn(void);
uint8_t MakeConstant(Value value);
uint8_t StoreConstant(Value value);
void WriteConstantOperation(Value value);
int WriteJump(uint8_t opcode);
void WriteLoop(int loopStart);
void PatchJump(int offset);

#endif
