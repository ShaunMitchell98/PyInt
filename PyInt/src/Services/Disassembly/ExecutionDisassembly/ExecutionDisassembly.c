#include "ExecutionDisassembly.h"
#include "../../../Types/String/String.h"
#include "../InstructionWriters/InstructionWriters.h"
#include "../Disassembly.h"
#include "../../../Types/Value/ValueFunctions.h"

static int WriteLocalInstruction(IOSettings* settings, const char* instructionName, char* buffer, int bufferSize, uint8_t constant, Value local, int offset) {
    strcat_s(buffer, bufferSize, instructionName);
    strcat_s(buffer, bufferSize, "\t\t ");
    char text[10] = "\0";
    _itoa_s(constant, text, 10, 10);
    strcat_s(buffer, bufferSize, text);
    strcat_s(buffer, bufferSize, "\t\t ");

    WriteValue(settings, local, OPERAND_VALUE, buffer, bufferSize);
    return offset + 2;
}

static int WriteExecutionInstruction(IOSettings* settings, Value* stack, Bytecode* bytecode, char* buffer, int bufferSize, int offset) {
    uint8_t instruction = bytecode->code[offset];
    uint8_t constant;
    Value value;

    switch (instruction) {
    case CONSTANT_OP: {
        constant = bytecode->code[offset + 1];
        offset = WriteInstructionWithConstant(settings, "CONSTANT_OP", buffer, bufferSize, constant, bytecode->constants.values[constant], offset);
        break;
    }
    case NONE_OP:
        offset = WriteInstructionWithoutConstant("NONE_OP", buffer, bufferSize, offset);
        break;
    case RETURN_OP:
        offset = WriteInstructionWithoutConstant("RETURN_OP", buffer, bufferSize, offset);
        break;
    case ADD_OP:
        offset = WriteInstructionWithoutConstant("ADD_OP", buffer, bufferSize, offset);
        break;
    case SUBTRACT_OP:
        offset = WriteInstructionWithoutConstant("SUBTRACT_OP", buffer, bufferSize, offset);
        break;
    case MULTIPLY_OP:
        offset = WriteInstructionWithoutConstant("MULTIPLY_OP", buffer, bufferSize, offset);
        break;
    case DIVIDE_OP:
        offset = WriteInstructionWithoutConstant("DIVIDE_OP", buffer, bufferSize, offset);
        break;
    case POWER_OP:
        offset = WriteInstructionWithoutConstant("POWER_OP", buffer, bufferSize, offset);
        break;
    case GREATER_OP:
        offset = WriteInstructionWithoutConstant("GREATER_OP", buffer, bufferSize, offset);
        break;
    case LESSER_OP:
        offset = WriteInstructionWithoutConstant("LESSER_OP", buffer, bufferSize, offset);
        break;
    case NOT_OP:
        offset = WriteInstructionWithoutConstant("NOT_OP", buffer, bufferSize, offset);
        break;
    case OR_OP:
        offset = WriteInstructionWithoutConstant("OR_OP", buffer, bufferSize, offset);
        break;
    case AND_OP:
        offset = WriteInstructionWithoutConstant("AND_OP", buffer, bufferSize, offset);
        break;
    case EQUAL_OP:
        offset = WriteInstructionWithoutConstant("EQUAL_OP", buffer, bufferSize, offset);
        break;
    case JUMP_IF_FALSE_OP:
        offset = WriteInstructionWithJump("JUMP_IF_FALSE_OP", buffer, bufferSize, bytecode, 1, offset);
        break;
    case JUMP_IF_TRUE_OP:
        offset = WriteInstructionWithJump("JUMP_IF_TRUE_OP", buffer, bufferSize, bytecode, 1, offset);
        break;
    case JUMP_OP:
        offset = WriteInstructionWithJump("JUMP_OP        ", buffer, bufferSize, bytecode, 1, offset);
        break;
    case POP_OP:
        offset = WriteInstructionWithoutConstant("POP_OP", buffer, bufferSize, offset);
        break;
    case LOOP_OP:
        offset = WriteInstructionWithJump("LOOP_OP", buffer, bufferSize, bytecode, -1, offset);
        break;
    case CALL_OP:
        constant = bytecode->code[offset + 1];
        offset = WriteInstructionWithConstant(settings, "CALL_OP", buffer, bufferSize, constant, *(stack+constant), offset);
        break;
    case CLASS_OP:
        constant = bytecode->code[offset + 1];
        offset = WriteInstructionWithConstant(settings, "CLASS_OP", buffer, bufferSize, constant, bytecode->constants.values[constant], offset);
        break;
    case INHERIT_OP:
        offset = WriteInstructionWithoutConstant("INHERIT_OP", buffer, bufferSize, offset);
        break;
    case GET_SUPERCLASS_OP:
        constant = bytecode->code[offset + 1];
        offset = WriteInstructionWithConstant(settings, "GET_SUPERCLASS_OP", buffer, bufferSize, constant, bytecode->constants.values[constant], offset);
        break;
    case CLOSURE_OP: {
        offset = WriteClosureOperation(settings, buffer, bufferSize, bytecode, offset);
        break;
    }
    case CLOSE_UPVALUE_OP: {
        offset = WriteInstructionWithoutConstant("CLOSE_UPVALUE_OP", buffer, bufferSize, offset);
        break;
    }
    case SET_PROPERTY_OP:
        constant = bytecode->code[offset + 1];
        offset = WriteInstructionWithConstant(settings, "SET_PROPERTY_OP", buffer, bufferSize, constant, bytecode->constants.values[constant], offset);
        break;
    case GET_PROPERTY_OP:
        constant = bytecode->code[offset + 1];
        offset = WriteInstructionWithConstant(settings, "GET_PROPERTY_OP", buffer, bufferSize, constant, bytecode->constants.values[constant], offset);
        break;
    case INVOKE_OP: 
        offset = WriteInvokeInstruction(settings, "INVOKE_OP", buffer, bufferSize, bytecode, offset);
        break;
    case INVOKE_SUPER_OP:
        offset = WriteInvokeInstruction(settings, "INVOKE_SUPER_OP", buffer, bufferSize, bytecode, offset);
        break;
    case SET_GLOBAL_OP:
        constant = bytecode->code[offset + 1];
        offset = WriteInstructionWithConstant(settings, "SET_GLOBAL_OP", buffer, bufferSize, constant, bytecode->constants.values[constant], offset);
        break;
    case GET_GLOBAL_OP:
        constant = bytecode->code[offset + 1];
        offset = WriteInstructionWithConstant(settings, "GET_GLOBAL_OP", buffer, bufferSize, constant, bytecode->constants.values[constant], offset);
        break;
    case SET_LOCAL_OP:
        constant = bytecode->code[offset + 1];
        value = *(stack + constant);
        offset = WriteLocalInstruction(settings, "SET_LOCAL_OP", buffer, bufferSize, constant, value, offset);
        break;
    case GET_LOCAL_OP:
        constant = bytecode->code[offset + 1];
        value = *(stack + constant);
        offset = WriteLocalInstruction(settings, "GET_LOCAL_OP", buffer, bufferSize, constant, value, offset);
        break;
    case GET_UPVALUE_OP:
        constant = bytecode->code[offset + 1];
        value = *(stack + constant);
        offset = WriteLocalInstruction(settings, "GET_UPVALUE_OP", buffer, bufferSize, constant, value, offset);
        break;
    case SET_UPVALUE_OP:
        constant = bytecode->code[offset + 1];
        value = *(stack + constant);
        offset = WriteLocalInstruction(settings, "SET_UPVALUE_OP", buffer, bufferSize, constant, value, offset);
        break;
    case END_OF_ARRAY_OP:
        constant = bytecode->code[offset + 1];
        value = *(stack + constant);
        offset = WriteLocalInstruction(settings, "END_OF_ARRAY_OP", buffer, bufferSize, constant, value, offset);
        break;
    case GET_INDEX_OP:
        constant = bytecode->code[offset + 1];
        value = *(stack + constant);
        offset = WriteLocalInstruction(settings, "GET_INDEX_OP", buffer, bufferSize, constant, value, offset);
        break;
    default:
        offset = WriteInstructionWithoutConstant("Unknown Opcode", buffer, bufferSize, offset);
        break;
    }

    return offset;
}

static int DisassembleExecutionInstruction(IOSettings* settings, Value* stack, Bytecode* bytecode, char* buffer, int bufferSize, int offset) {
    strcat_s(buffer, bufferSize, "\t");
    WriteInstructionAddress(buffer, bufferSize, offset);

    strcat_s(buffer, bufferSize, "\t\t");
    WriteLineNumber(bytecode, buffer, bufferSize, offset);

    strcat_s(buffer, bufferSize, "\t  ");
    offset = WriteExecutionInstruction(settings, stack, bytecode, buffer, bufferSize, offset);

    return offset;
}

static void WriteExecutionLineDivider(char* buffer, int bufferSize) {
    strcat_s(buffer, bufferSize, "\n-----------------------------------------------------------------------------------------------------------------------------\n");
}

void InitialiseExecutionDisassembly(IOSettings* executionSettings) {
    int bufferSize = 500 * sizeof(char);
    char* buffer = (char*)malloc(bufferSize);
    *buffer = '\0';

    strcat_s(buffer, bufferSize, "Execution Readout \n");
    WriteExecutionLineDivider(buffer, bufferSize);
    strcat_s(buffer, bufferSize, "Instruction Address   Line        Instruction         Operand address    Operand value           Stack");
    WriteExecutionLineDivider(buffer, bufferSize);

    HandleOutput(buffer, executionSettings);
}

void DisassembleExecution(Bytecode* bytecode, int offset, Value* stack, Value* stackTop, IOSettings* executionSettings) {
    int bufferSize = 2000 * sizeof(char);
    char* buffer = (char*)malloc(bufferSize);
    *buffer = '\0';

    int newOffset = DisassembleExecutionInstruction(executionSettings, stack, bytecode, buffer, bufferSize, offset);

    char stackWhitespace[10] = "";

    if (newOffset == offset + 1) {
        strcpy_s(stackWhitespace, 10, "\t\t\t\t\t\t\t");
    }
    else if (newOffset == offset + 2) {
        strcpy_s(stackWhitespace, 10, "\t\t\t");
    }
    else if (newOffset == offset + 3) {
        strcpy_s(stackWhitespace, 10, "\t\t");
    }

    strcat_s(buffer, bufferSize, stackWhitespace);

    DisassembleStack(executionSettings, stack, stackTop, buffer, bufferSize);

    if (newOffset >= bytecode->count) {
        strcat_s(buffer, bufferSize, "--------------------------------------------------------------------------------------------------------------------------- \n \n");
    }

    HandleOutput(buffer, executionSettings);
}