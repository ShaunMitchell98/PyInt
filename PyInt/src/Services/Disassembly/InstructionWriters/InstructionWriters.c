#include <string.h>

// Include our platform compatibility header
#include "platform_compat.h"

#include "InstructionWriters.h"
#include "../../../Types/Value/ValueFunctions.h"

int WriteInstructionWithoutConstant(const char* instructionName, char* buffer, int bufferSize, int offset) {
    PYINT_STRCAT(buffer, bufferSize, instructionName);
    return offset + 1;
}

int WriteInstructionWithConstant(IOSettings* settings, const char* instructionName, char* buffer, int bufferSize, uint8_t constant, Value value, int offset) {
    PYINT_STRCAT(buffer, bufferSize, instructionName);
    PYINT_STRCAT(buffer, bufferSize, "\t\t ");
    char text[10] = "\0";
    PYINT_ITOA(constant, text, 10, 10);
    PYINT_STRCAT(buffer, bufferSize, text);
    PYINT_STRCAT(buffer, bufferSize, "\t\t ");
    WriteValue(settings, value, OPERAND_VALUE, buffer, bufferSize);
    return offset + 2;
}

int WriteClosureOperation(IOSettings* settings, char* buffer, int bufferSize, Bytecode* bytecode, int offset) {
    offset++;
    uint8_t constant = bytecode->code[offset++];
    PYINT_STRCAT(buffer, bufferSize, "CLOSURE_OP");
    PYINT_STRCAT(buffer, bufferSize, "\t\t ");
    char text[10] = "\0";
    PYINT_ITOA(constant, text, 10, 10);
    PYINT_STRCAT(buffer, bufferSize, text);
    PYINT_STRCAT(buffer, bufferSize, "\t\t ");
    WriteValue(settings, bytecode->constants.values[constant], OPERAND_VALUE, buffer, bufferSize);
    PYINT_STRCAT(buffer, bufferSize, "\t");

    Function* function = AS_FUNCTION(bytecode->constants.values[constant]);
    for (int j = 0; j < function->upvalueCount; j++) {
        int isLocal = bytecode->code[offset++];
        int index = bytecode->code[offset++];
        /*char newText[10] = "\0";*/
        // Commented out code updated to use PYINT_ macros
        //PYINT_ITOA(offset - 2, newText, 10, 10);
        //PYINT_STRCAT(buffer, bufferSize, newText);
        //PYINT_STRCAT(buffer, bufferSize, "     |           ");
        //PYINT_STRCAT(buffer, bufferSize, isLocal ? "local" : "upvalue");
        //char newText2[10] = "\0";
        //PYINT_ITOA(index, newText2, 10, 10);
        //PYINT_STRCAT(buffer, bufferSize, newText2);
        //PYINT_STRCAT(buffer, bufferSize, "\n");
    }

    return offset;
}

int WriteInstructionWithJump(const char* instructionName, char* buffer, int bufferSize, Bytecode* bytecode, int sign, int offset) {
    uint16_t jump = (uint16_t)(bytecode->code[offset + 1] << 8);
    jump |= bytecode->code[offset + 2];
    PYINT_STRCAT(buffer, bufferSize, instructionName);
    PYINT_STRCAT(buffer, bufferSize, "\t\t\t ");
    char text[10] = "\0";
    PYINT_ITOA(offset, text, 10, 10);
    PYINT_STRCAT(buffer, bufferSize, text);
    PYINT_STRCAT(buffer, bufferSize, " -> ");
    PYINT_ITOA(offset + 3 + sign * jump, text, 10, 10);
    PYINT_STRCAT(buffer, bufferSize, text);
    return offset + 3;
}

int WriteInvokeInstruction(IOSettings* settings, const char* instructionName, char* buffer, int bufferSize, Bytecode* bytecode, int offset) {
    uint8_t constant = bytecode->code[offset + 1];
    uint8_t argCount = bytecode->code[offset + 2];
    PYINT_STRCAT(buffer, bufferSize, instructionName);
    PYINT_STRCAT(buffer, bufferSize, " (");

    char text[10] = "\0";
    PYINT_ITOA(argCount, text, 10, 10);
    PYINT_STRCAT(buffer, bufferSize, text);
    PYINT_STRCAT(buffer, bufferSize, " args) ");

    PYINT_ITOA(constant, text, 10, 10);
    PYINT_STRCAT(buffer, bufferSize, text);
    PYINT_STRCAT(buffer, bufferSize, " ");

    WriteValue(settings, bytecode->constants.values[constant], OPERAND_VALUE, buffer, bufferSize);
    return offset + 3;
}

void DisassembleStack(IOSettings* settings, Value* stack, Value* stackTop, char* buffer, int bufferSize) {
    for (Value* slot = stack; slot < stackTop; slot++) {
        PYINT_STRCAT(buffer, bufferSize, "[");
        WriteValue(settings, *slot, STACK, buffer, bufferSize);
        PYINT_STRCAT(buffer, bufferSize, "] ");
    }
    PYINT_STRCAT(buffer, bufferSize, "\n");
}

void WriteInstructionAddress(char* buffer, int bufferSize, int offset) {
    char text[10];
    PYINT_ITOA(offset, text, 10, 10);
    PYINT_STRCAT(buffer, bufferSize, text);
}

void WriteLineNumber(Bytecode* bytecode, char* buffer, int bufferSize, int offset) {
    if (offset > 0 && bytecode->lines[offset] == bytecode->lines[offset - 1]) {
        PYINT_STRCAT(buffer, bufferSize, "|");
    }
    else {
        char text[10];
        PYINT_ITOA(bytecode->lines[offset], text, 10, 10);
        PYINT_STRCAT(buffer, bufferSize, text);
    }
}