#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../Headers/Debug.h"
#include "../Headers/Value.h"
#include "../Headers/VM.h"
#include "../Headers/PrintType.h"

static int WriteInstructionWithoutConstant(const char* instructionName, char* buffer, int bufferSize, int offset) {
     strcat_s(buffer, bufferSize, instructionName);
     return offset + 1;
}

static int WriteInstructionWithConstant(IOSettings* settings, const char* instructionName, char* buffer, int bufferSize, uint8_t constant, Value value, int offset) {
      strcat_s(buffer, bufferSize, instructionName); 
      strcat_s(buffer, bufferSize, "\t\t ");
      char text[10] = "\0";
      _itoa_s(constant, text, 10, 10);
      strcat_s(buffer, bufferSize, text);
      strcat_s(buffer, bufferSize, "\t\t ");
      WriteValue(settings, value, OPERAND_VALUE, buffer, bufferSize);
      return offset + 2;
}

static int WriteInstructionWithJump(const char* instructionName, char* buffer, int bufferSize, Bytecode* bytecode, int sign, int offset) {
    uint16_t jump = (uint16_t)(bytecode->code[offset+1] << 8);
    jump |= bytecode->code[offset+2];
    strcat_s(buffer, bufferSize, instructionName);
    strcat_s(buffer, bufferSize, "\t\t\t ");
    char text[10] = "\0";
    _itoa_s(offset, text, 10, 10);
    strcat_s(buffer, bufferSize, text);
    strcat_s(buffer, bufferSize, " -> ");
    _itoa_s(offset + 3 + sign * jump, text, 10, 10);
    strcat_s(buffer, bufferSize, text);
    return offset+3;
}

static void DisassembleStack(IOSettings* settings, Value* stack, Value* stackTop, char* buffer, int bufferSize) {
    for (Value* slot = stack; slot < stackTop; slot++) {
        strcat_s(buffer, bufferSize, "[");
        WriteValue(settings, *slot, STACK, buffer, bufferSize);
        strcat_s(buffer, bufferSize, "] ");
    }
    strcat_s(buffer, bufferSize, "\n");
}

static void WriteInstructionAddress(char* buffer, int bufferSize, int offset) {
    char text[10];
    _itoa_s(offset, text, 10, 10);
    strcat_s(buffer, bufferSize, text);
}

static void WriteLineNumber(Bytecode *bytecode, char* buffer, int bufferSize, int offset) {
    if (offset > 0 && bytecode->lines[offset] == bytecode->lines[offset-1]) {
        strcat_s(buffer, bufferSize, "|");
    }
    else {
        char text[10];
        _itoa_s(bytecode->lines[offset], text, 10, 10);
        strcat_s(buffer, bufferSize, text);
    }
}

static int WriteInstruction(IOSettings* settings, VM* vm, Bytecode *bytecode, char* buffer, int bufferSize, int offset) {
    uint8_t instruction = bytecode->code[offset];
    uint8_t constant;
    
    switch (instruction) {
        case CONSTANT_OP: {
            constant = bytecode->code[offset+1];
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
            offset = WriteInstructionWithJump("JUMP_OP", buffer, bufferSize, bytecode, 1, offset);
            break;
        case POP_OP:
            offset = WriteInstructionWithoutConstant("POP_OP", buffer, bufferSize, offset);
            break;
        case PRINT_OP:
            offset = WriteInstructionWithoutConstant("PRINT_OP", buffer, bufferSize, offset);
            break;
        case LOOP_OP:
            offset = WriteInstructionWithJump("LOOP_OP", buffer, bufferSize, bytecode, -1, offset);
            break;
        case CALL_OP:
            constant = bytecode->code[offset + 1];
            offset = WriteInstructionWithConstant(settings, "CALL_OP", buffer, bufferSize, constant, bytecode->constants.values[constant], offset);
            break;
        case SET_GLOBAL_OP:
            constant = bytecode->code[offset+1];
            offset = WriteInstructionWithConstant(settings, "SET_GLOBAL_OP", buffer, bufferSize, constant, bytecode->constants.values[constant], offset);
            break;
        case GET_GLOBAL_OP:
            constant = bytecode->code[offset+1];
            offset = WriteInstructionWithConstant(settings, "GET_GLOBAL_OP", buffer, bufferSize, constant, bytecode->constants.values[constant], offset);
            break;
        case SET_LOCAL_OP:
            constant = bytecode->code[offset+1];
            offset= WriteInstructionWithConstant(settings, "SET_LOCAL_OP", buffer, bufferSize, constant, vm->stack[constant], offset);
            break;
        case GET_LOCAL_OP:
            constant = bytecode->code[offset+1];
            offset = WriteInstructionWithConstant(settings, "GET_LOCAL_OP", buffer, bufferSize, constant, vm->stack[constant], offset);
            break;
        case END_OF_ARRAY_OP:
            constant = bytecode->code[offset+1];
            offset = WriteInstructionWithConstant(settings, "END_OF_ARRAY_OP", buffer, bufferSize, constant, vm->stack[constant], offset);
            break;
        case GET_INDEX_OP:
            constant = bytecode->code[offset+1];
            offset = WriteInstructionWithConstant(settings, "GET_INDEX_OP", buffer, bufferSize, constant, vm->stack[constant], offset);
            break;
        default:
            offset = WriteInstructionWithoutConstant("Unknown Opcode", buffer, bufferSize, offset);
            break;
        }

    return offset;
}


static int DisassembleInstruction(IOSettings* settings, VM* vm, Bytecode* bytecode, char* buffer, int bufferSize, int offset) {
    strcat_s(buffer, bufferSize, "\t");
    WriteInstructionAddress(buffer, bufferSize, offset);

    strcat_s(buffer, bufferSize, "\t\t");
    WriteLineNumber(bytecode, buffer, bufferSize, offset); 
    
    strcat_s(buffer, bufferSize, "\t  ");
    offset = WriteInstruction(settings, vm, bytecode, buffer, bufferSize, offset); 
    
    return offset;
}

static void DisassembleToTerminal(char* buffer) {
    printf(buffer);

}

static void DisassembleToFile(char* buffer, const char* filePath) {
    FILE file;
    FILE* fp = &file;
    errno_t err = fopen_s(&fp, filePath, "a");

    if (err != 0) {
        fprintf(stderr, "Can't open bytecode disassembly output file: %s", filePath);
        return;
    }

    fputs(buffer, fp);
    fclose(fp);
}

static void WriteBytecodeLineDivider(char* buffer, int bufferSize) {
    strcat_s(buffer, bufferSize, "\n------------------------------------------------------------------------------------------\n");
}

static void WriteExecutionLineDivider(char* buffer, int bufferSize) {
    strcat_s(buffer, bufferSize, "\n------------------------------------------------------------------------------------------------------------------\n");
}

static void HandleOutput(char* buffer, IOSettings* settings) {
    if (settings->location == LOCATION_FILE) {
        DisassembleToFile(buffer, settings->filePath);
        free(buffer);
    }
    else if (settings->location == LOCATION_TERMINAL) {
        DisassembleToTerminal(buffer);
        free(buffer);
    }
    else if (settings->location == LOCATION_STRING) {
        settings->string = buffer;
    }
}

void DisassembleBytecode(VM* vm, Bytecode* bytecode, const char* functionName, IOSettings* bytecodeSettings) {
    int bufferSize = 2000 * sizeof(char);
    char* buffer = (char*)malloc(bufferSize);
    *buffer = '\0';

    strcat_s(buffer, bufferSize, "Bytecode Readout - ");
    strcat_s(buffer, bufferSize, functionName);
    strcat_s(buffer, bufferSize, "\n");
    WriteBytecodeLineDivider(buffer, bufferSize);
    strcat_s(buffer, bufferSize, "Instruction Address   Line        Instruction         Operand address    Operand value");
    WriteBytecodeLineDivider(buffer, bufferSize);

    for (int offset = 0; offset < bytecode->count;) {
        offset = DisassembleInstruction(&vm->settings.bytecode, vm, bytecode, buffer, bufferSize, offset);
        strcat_s(buffer, bufferSize, "\n");
    }

    WriteBytecodeLineDivider(buffer, bufferSize);
    HandleOutput(buffer, bytecodeSettings);
 
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

void DisassembleExecution(VM* vm, Bytecode* bytecode, int offset, Value* stack, Value* stackTop, IOSettings* executionSettings) {
    int bufferSize = 2000 * sizeof(char);
    char* buffer = (char*)malloc(bufferSize);
    *buffer = '\0';

    int newOffset = DisassembleInstruction(&vm->settings.execution, vm, bytecode, buffer, bufferSize, offset);

    char stackWhitespace[10] = "\0";

    if (newOffset == offset + 1) {
        strcpy_s(stackWhitespace, 10, "\t\t\t\t\t\t\t");
    }
    else if (newOffset == offset + 2) {
        strcpy_s(stackWhitespace, 10, "\t\t");
    }
    else if (newOffset == offset + 3) {
        strcpy_s(stackWhitespace, 10, "\t\t");
    }

    strcat_s(buffer, bufferSize, stackWhitespace);

    DisassembleStack(&vm->settings.execution, stack, stackTop, buffer, bufferSize);
    
    if (newOffset >= bytecode->count) {
        strcat_s(buffer, bufferSize, "------------------------------------------------------------------------ \n \n");
    }

    HandleOutput(buffer, executionSettings);
}
