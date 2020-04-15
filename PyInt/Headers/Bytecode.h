#ifndef PyInt_Bytecode_h
#define PyInt_Bytecode_h

#include "Value.h"
#include "Common.h"

typedef enum {
    EQUAL_OP,
    NOT_OP,
    OR_OP,
    AND_OP,
    NEGATE_OP,
    RETURN_OP,
    CONSTANT_OP,
    ADD_OP,
    SUBTRACT_OP,
    MULTIPLY_OP,
    DIVIDE_OP,
    POWER_OP,
    GREATER_OP,
    LESSER_OP,
    GET_LOCAL_OP,
    SET_LOCAL_OP,
    JUMP_IF_FALSE_OP,
    JUMP_IF_TRUE_OP,
    POP_OP,
    JUMP_OP,
    LOOP_OP,
    SET_GLOBAL_OP,
    GET_GLOBAL_OP,
    NONE_OP,
    GET_INDEX_OP,
    END_OF_ARRAY_OP,
    CALL_OP
} Opcode;

typedef struct {
    int count;
    int capacity;
    uint8_t* code;
	int* lines;
	ValueArray constants;
} Bytecode;


void InitBytecode(Bytecode* bytecode);
void FreeBytecode(Bytecode* bytecode);
void WriteBytecode(Bytecode* bytecode, uint8_t byte, int line);
int AddConstantToValueArray(Bytecode* bytecode, Value value);

#endif
