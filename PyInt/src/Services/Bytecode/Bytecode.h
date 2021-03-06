#ifndef PyInt_Bytecode_h
#define PyInt_Bytecode_h

#include "../../Headers/Common.h"
#include "../../Types/Value/Value.h"

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
    GET_UPVALUE_OP,
    SET_UPVALUE_OP,
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
    CALL_OP,
    CLOSURE_OP,
    CLOSE_UPVALUE_OP,
    CLASS_OP,
    SET_PROPERTY_OP,
    GET_PROPERTY_OP,
    INVOKE_OP,
    INHERIT_OP,
    GET_SUPERCLASS_OP,
    INVOKE_SUPER_OP
} Opcode;

typedef struct {
    int count;
    int capacity;
    uint8_t* code;
	int* lines;
	ValueArray constants;
} Bytecode;

#endif
