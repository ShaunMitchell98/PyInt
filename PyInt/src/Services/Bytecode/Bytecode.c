#include <stdlib.h>

#include "Bytecode.h"
#include "../Memory/Memory.h"

void InitBytecode(Bytecode* bytecode) {
	bytecode->count = 0;
	bytecode->capacity = 0;
	bytecode->code = NULL;
	bytecode->lines = NULL;
	InitValueArray(&bytecode->constants);
}

void FreeBytecode(Bytecode* bytecode) {
	FREE_ARRAY(uint8_t, bytecode->code, bytecode->count);
	FREE_ARRAY(int, bytecode->lines, bytecode->count);
	FREE(Bytecode, bytecode);
}

void WriteBytecode(Bytecode* bytecode, uint8_t byte, int line) {
	if (bytecode->capacity < bytecode->count + 1) {
		int oldCapacity = bytecode->capacity;
		bytecode->capacity = GROW_CAPACITY(bytecode->capacity);
		bytecode->code = GROW_ARRAY(bytecode->code, uint8_t, oldCapacity, bytecode->capacity);
		bytecode->lines = GROW_ARRAY(bytecode->lines, int, oldCapacity, bytecode->capacity);
	}
	bytecode->code[bytecode->count] = byte;
	bytecode->lines[bytecode->count] = line;
	bytecode->count++;
}

int AddConstantToValueArray(Bytecode* bytecode, Value value) {
	WriteValueArray(&bytecode->constants, value);
	return bytecode->constants.count-1;
}
