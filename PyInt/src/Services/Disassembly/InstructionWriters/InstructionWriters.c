#include "InstructionWriters.h"

int WriteInstructionWithoutConstant(const char* instructionName, char* buffer, int bufferSize, int offset) {
    strcat_s(buffer, bufferSize, instructionName);
    return offset + 1;
}

int WriteInstructionWithConstant(IOSettings* settings, const char* instructionName, char* buffer, int bufferSize, uint8_t constant, Value value, int offset) {
    strcat_s(buffer, bufferSize, instructionName);
    strcat_s(buffer, bufferSize, "\t\t ");
    char text[10] = "\0";
    _itoa_s(constant, text, 10, 10);
    strcat_s(buffer, bufferSize, text);
    strcat_s(buffer, bufferSize, "\t\t ");
    WriteValue(settings, value, OPERAND_VALUE, buffer, bufferSize);
    return offset + 2;
}

int WriteClosureOperation(IOSettings* settings, char* buffer, int bufferSize, Bytecode* bytecode, int offset) {
    offset++;
    uint8_t constant = bytecode->code[offset++];
    strcat_s(buffer, bufferSize, "CLOSURE_OP");
    strcat_s(buffer, bufferSize, "\t\t ");
    char text[10] = "\0";
    _itoa_s(constant, text, 10, 10);
    strcat_s(buffer, bufferSize, text);
    strcat_s(buffer, bufferSize, "\t\t ");
    WriteValue(settings, bytecode->constants.values[constant], OPERAND_VALUE, buffer, bufferSize);
    strcat_s(buffer, bufferSize, "\t");

    Function* function = AS_FUNCTION(bytecode->constants.values[constant]);
    for (int j = 0; j < function->upvalueCount; j++) {
        int isLocal = bytecode->code[offset++];
        int index = bytecode->code[offset++];
        /*char newText[10] = "\0";*/
        //_itoa_s(offset - 2, newText, 10, 10);
        //strcat_s(buffer, bufferSize, newText);
        //strcat_s(buffer, bufferSize, "     |           ");
        //strcat_s(buffer, bufferSize, isLocal ? "local" : "upvalue");
        //char newText2[10] = "\0";
        //_itoa_s(index, newText2, 10, 10);
        //strcat_s(buffer, bufferSize, newText2);
        //strcat_s(buffer, bufferSize, "\n");
    }

    return offset;
}

int WriteInstructionWithJump(const char* instructionName, char* buffer, int bufferSize, Bytecode* bytecode, int sign, int offset) {
    uint16_t jump = (uint16_t)(bytecode->code[offset + 1] << 8);
    jump |= bytecode->code[offset + 2];
    strcat_s(buffer, bufferSize, instructionName);
    strcat_s(buffer, bufferSize, "\t\t\t ");
    char text[10] = "\0";
    _itoa_s(offset, text, 10, 10);
    strcat_s(buffer, bufferSize, text);
    strcat_s(buffer, bufferSize, " -> ");
    _itoa_s(offset + 3 + sign * jump, text, 10, 10);
    strcat_s(buffer, bufferSize, text);
    return offset + 3;
}

void DisassembleStack(IOSettings* settings, Value* stack, Value* stackTop, char* buffer, int bufferSize) {
    for (Value* slot = stack; slot < stackTop; slot++) {
        strcat_s(buffer, bufferSize, "[");
        WriteValue(settings, *slot, STACK, buffer, bufferSize);
        strcat_s(buffer, bufferSize, "] ");
    }
    strcat_s(buffer, bufferSize, "\n");
}

void WriteInstructionAddress(char* buffer, int bufferSize, int offset) {
    char text[10];
    _itoa_s(offset, text, 10, 10);
    strcat_s(buffer, bufferSize, text);
}

void WriteLineNumber(Bytecode* bytecode, char* buffer, int bufferSize, int offset) {
    if (offset > 0 && bytecode->lines[offset] == bytecode->lines[offset - 1]) {
        strcat_s(buffer, bufferSize, "|");
    }
    else {
        char text[10];
        _itoa_s(bytecode->lines[offset], text, 10, 10);
        strcat_s(buffer, bufferSize, text);
    }
}