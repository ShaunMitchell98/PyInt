#include <stdio.h>

#include "Debug.h"
#include "Value.h"
#include "VM.h"
#include "PrintType.h"

static int PrintWithoutConstant(const char* instructionName, int offset) {
     printf(" \t%-16s  \t  ", instructionName);
     return offset + 1;
}

static int PrintWithConstant(const char* instructionName, uint8_t constant, Value value, int offset) {
       printf(" \t%-16s  \t  %d ", instructionName, constant);
      PrintValue(value, OPERAND_VALUE);
      return offset + 2;
}

static int PrintJump(const char* instructionName, Bytecode* bytecode, int sign,  int offset) {
    uint16_t jump = (uint16_t)(bytecode->code[offset+1] << 8);
    jump |= bytecode->code[offset+2];
    printf(" \t%-16s %4d -> %d", instructionName, offset, offset+3+sign*jump);
    return offset+3;
}

static void DisassembleStack(Value* stack, Value* stackTop, bool hadOperand) {
    if (hadOperand) {
        printf("\t\t ");
        for (Value* slot = stack; slot < stackTop; slot++) {
            printf("[");
            PrintValue(*slot, STACK);
            printf("]");
        }
        if (stack == stackTop) {
            printf("[]");
        }
        printf("\n");
    }
    else {
        printf("\t\t\t\t\t\t\t ");
        for (Value* slot = stack; slot < stackTop; slot++) {
            printf("[");
            PrintValue(*slot, STACK);
            printf("] ");
        }
        printf("\n");
    }
}

static void PrintLineNumber(Bytecode *bytecode, int offset) {
    if (offset > 0 && bytecode->lines[offset] == bytecode->lines[offset-1]) {
        printf("\t\t\t|");
    }
    else {
        printf("\t\t %4d ", bytecode->lines[offset]);
    }
}

static int PrintInstruction(Bytecode *bytecode, int offset) {
    uint8_t instruction = bytecode->code[offset];
    uint8_t constant;
    
    switch (instruction) {
        case CONSTANT_OP: {
            constant = bytecode->code[offset+1];
            offset = PrintWithConstant("CONSTANT_OP", constant, bytecode->constants.values[constant], offset);
            break;
        }
        case NONE_OP:
            offset = PrintWithoutConstant("NONE_OP", offset);
            break;
        case RETURN_OP:
            offset = PrintWithoutConstant("RETURN_OP", offset);
            break;
        case ADD_OP:
            offset = PrintWithoutConstant("ADD_OP", offset);
            break;
        case SUBTRACT_OP:
            offset = PrintWithoutConstant("SUBTRACT_OP", offset);
            break;
        case MULTIPLY_OP:
            offset = PrintWithoutConstant("MULTIPLY_OP", offset);
            break;
        case DIVIDE_OP:
            offset = PrintWithoutConstant("DIVIDE_OP", offset);
            break;
        case POWER_OP:
            offset = PrintWithoutConstant("POWER_OP", offset);
            break;
        case GREATER_OP:
            offset = PrintWithoutConstant("GREATER_OP", offset);
            break;
        case LESSER_OP:
            offset = PrintWithoutConstant("LESSER_OP", offset);
            break;
        case NOT_OP:
            offset = PrintWithoutConstant("NOT_OP", offset);
            break;
        case OR_OP:
            offset = PrintWithoutConstant("OR_OP", offset);
            break;
        case AND_OP:
            offset = PrintWithoutConstant("AND_OP", offset);
            break;
        case EQUAL_OP:
            offset = PrintWithoutConstant("EQUAL_OP", offset);
            break;
        case JUMP_IF_FALSE_OP:
            offset = PrintJump("JUMP_IF_FALSE_OP", bytecode, 1, offset);
            break;
        case JUMP_IF_TRUE_OP:
            offset = PrintJump("JUMP_IF_TRUE_OP", bytecode, 1, offset);
            break;
        case JUMP_OP:
            offset = PrintJump("JUMP_OP", bytecode, 1, offset);
            break;
        case POP_OP:
            offset = PrintWithoutConstant("POP_OP", offset);
            break;
        case PRINT_OP:
            offset = PrintWithoutConstant("PRINT_OP", offset);
            break;
        case LOOP_OP:
            offset = PrintJump("LOOP_OP", bytecode, -1, offset);
            break;
        case DEFINE_GLOBAL_OP:
            constant = bytecode->code[offset+1];
            offset = PrintWithConstant("DEFINE_GLOBAL_OP", constant, bytecode->constants.values[constant], offset);
            break;
        case SET_GLOBAL_OP:
            constant = bytecode->code[offset+1];
            offset = PrintWithConstant("SET_GLOBAL_OP", constant, bytecode->constants.values[constant], offset);
            break;
        case GET_GLOBAL_OP:
            constant = bytecode->code[offset+1];
            offset = PrintWithConstant("GET_GLOBAL_OP", constant, bytecode->constants.values[constant], offset);
            break;
        case SET_LOCAL_OP:
            constant = bytecode->code[offset+1];
            offset= PrintWithConstant("SET_LOCAL_OP", constant, vm.stack[constant], offset);
            break;
        case GET_LOCAL_OP:
            constant = bytecode->code[offset+1];
            offset = PrintWithConstant("GET_LOCAL_OP", constant, vm.stack[constant], offset);
            break;
        case DECLARE_LOCAL_OP:
            constant = bytecode->code[offset+1];
            offset = PrintWithConstant("DECLARE_LOCAL_OP", constant, vm.stack[constant], offset);
            break;
        case END_OF_ARRAY_OP:
            constant = bytecode->code[offset+1];
            offset = PrintWithConstant("END_OF_ARRAY_OP", constant, vm.stack[constant], offset);
            break;
        case GET_INDEX_OP:
            constant = bytecode->code[offset+1];
            offset = PrintWithConstant("GET_INDEX_OP", constant, vm.stack[constant], offset);
            break;
        default:
            offset = PrintWithoutConstant("Unknown Opcode", offset);
            break;
        }

    return offset;
    }


static int DisassembleInstruction(Bytecode* bytecode, int offset) {
	printf("\t\t%04d ", offset); //Print offset

    PrintLineNumber(bytecode, offset); //Print line number

    offset = PrintInstruction(bytecode, offset); //Print Instruction
    
    return offset;
}

void DisassembleBytecode(Bytecode* bytecode) {
    printf("Bytecode Readout \n");
    printf("Instruction Address   Line  Instruction   Operand address    Operand value\n");

    for (int offset = 0; offset < bytecode->count;) {
        offset = DisassembleInstruction(bytecode, offset);
        printf("\n");
    }
    
    printf("----------------------------------------------------------------------------- \n \n");
}

void DisassembleExecution(Bytecode* bytecode, int offset, Value* stack, Value* stackTop) {
 
    int newOffset = DisassembleInstruction(bytecode, offset);
    
    if (newOffset == offset+2) {
        DisassembleStack(stack, stackTop, true);
    }
    else {
        DisassembleStack(stack, stackTop, false);
    }
    
    if (newOffset >= bytecode->count) {
        printf("------------------------------------------------------------------------ \n \n");
    }
}
