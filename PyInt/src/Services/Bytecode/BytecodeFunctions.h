#ifndef PyInt_BytecodeFunctions_h
#define PyInt_BytecodeFunctions_h

#include "Bytecode.h"
#include "../GarbageCollection/GarbageCollector/GarbageCollector.h"

void InitBytecode(Bytecode* bytecode);
void FreeBytecode(GarbageCollector* garbageCollector, Bytecode* bytecode);
void WriteBytecode(GarbageCollector* garbageCollector, Bytecode* bytecode, uint8_t byte, int line);
int AddConstantToValueArray(GarbageCollector* garbageCollector, Bytecode* bytecode, Value value);

#endif