#include <stdlib.h>

#include "Bytecode.h"
#include "BytecodeFunctions.h"
#include "../Memory/Memory.h"
#include "../../Types/Value/ValueFunctions.h"

void InitBytecode(Bytecode* bytecode) {
	bytecode->count = 0;
	bytecode->capacity = 0;
	bytecode->code = NULL;
	bytecode->lines = NULL;
	InitValueArray(&bytecode->constants);
}

void FreeBytecode(GarbageCollector* garbageCollector, Bytecode* bytecode) {
	FREE_ARRAY(garbageCollector, uint8_t, bytecode->code, bytecode->count);
	FREE_ARRAY(garbageCollector, int, bytecode->lines, bytecode->count);
	FreeValueArray(garbageCollector, &bytecode->constants);
	InitBytecode(bytecode);
}

void WriteBytecode(GarbageCollector* garbageCollector, Bytecode* bytecode, uint8_t byte, int line) {
	if (bytecode->capacity < bytecode->count + 1) {
		int oldCapacity = bytecode->capacity;
		bytecode->capacity = GROW_CAPACITY(bytecode->capacity);
		bytecode->code = GROW_ARRAY(garbageCollector, bytecode->code, uint8_t, oldCapacity, bytecode->capacity);
		bytecode->lines = GROW_ARRAY(garbageCollector, bytecode->lines, int, oldCapacity, bytecode->capacity);
	}
	bytecode->code[bytecode->count] = byte;
	bytecode->lines[bytecode->count] = line;
	bytecode->count++;
}

int AddConstantToValueArray(GarbageCollector* garbageCollector, Bytecode* bytecode, Value value) {
	WriteValueArray(garbageCollector, &bytecode->constants, value);
	return bytecode->constants.count-1;
}
