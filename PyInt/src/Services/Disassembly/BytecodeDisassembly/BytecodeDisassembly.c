#include "BytecodeDisassembly.h"
#include "../InstructionWriters/InstructionWriters.h"
#include "../Disassembly.h"
#include "../../../Compilation/Variables/Upvalue/UpvalueResolver.h"

static int WriteLocalInstruction(const char* instructionName, char* buffer, int bufferSize, uint8_t constant, const char* localName, int localLength, int offset) {
    strcat_s(buffer, bufferSize, instructionName);
    strcat_s(buffer, bufferSize, "\t\t ");
    char text[10] = "\0";
    _itoa_s(constant, text, 10, 10);
    strcat_s(buffer, bufferSize, text);
    strcat_s(buffer, bufferSize, "\t\t ");

    char* name = (char*)malloc(localLength + 1);
    memcpy(name, localName, localLength);
    name[localLength] = '\0';

    strcat_s(buffer, bufferSize, name);
    free(name);
    return offset + 2;
}

static WriteUpvalueInstruction(const char* instructionName, char* buffer, int bufferSize, uint8_t constant, CompilerUpvalue* upvalue, Compiler* compiler, int offset) {
    strcat_s(buffer, bufferSize, instructionName);
    strcat_s(buffer, bufferSize, "\t ");
    char text[10] = "\0";
    _itoa_s(constant, text, 10, 10);
    strcat_s(buffer, bufferSize, text);
    strcat_s(buffer, bufferSize, "\t\t ");

    Token* token = GetUpvalueToken(compiler, upvalue->index, upvalue->isLocal);
    char* name = (char*)malloc(token->length + 1);
    memcpy(name, token->start, token->length);
    name[token->length] = '\0';

    strcat_s(buffer, bufferSize, name);
    free(name);
    return offset + 2;
}

static int WriteBytecodeInstruction(Compiler* compiler, IOSettings* settings, Local* stack, CompilerUpvalue* upvalues, Bytecode* bytecode, char* buffer, int bufferSize, int offset) {
    uint8_t instruction = bytecode->code[offset];
    uint8_t constant;

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
        offset = WriteInstructionWithConstant(settings, "CALL_OP", buffer, bufferSize, constant, bytecode->constants.values[constant], offset);
        break;
    case CLOSURE_OP: {
        offset = WriteClosureOperation(settings, buffer, bufferSize, bytecode, offset);
        break;
    }
    case CLOSE_UPVALUE_OP: {
        offset = WriteInstructionWithoutConstant("CLOSE_UPVALUE_OP", buffer, bufferSize, offset);
        break;
    }
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
        offset = WriteLocalInstruction("SET_LOCAL_OP", buffer, bufferSize, constant, (stack + constant)->name.start, (stack + constant)->name.length, offset);
        break;
    case GET_LOCAL_OP:
        constant = bytecode->code[offset + 1];
        offset = WriteLocalInstruction("GET_LOCAL_OP", buffer, bufferSize, constant, (stack + constant)->name.start, (stack + constant)->name.length, offset);
        break;
    case GET_UPVALUE_OP:
        constant = bytecode->code[offset + 1];
        offset = WriteUpvalueInstruction("GET_UPVALUE_OP", buffer, bufferSize, constant, (upvalues + constant), compiler, offset);
        break;
    case SET_UPVALUE_OP:
        constant = bytecode->code[offset + 1];
        offset = WriteUpvalueInstruction("SET_UPVALUE_OP", buffer, bufferSize, constant, (upvalues + constant), compiler, offset);
        break;
    case END_OF_ARRAY_OP:
        constant = bytecode->code[offset + 1];
        offset = WriteLocalInstruction("END_OF_ARRAY_OP", buffer, bufferSize, constant, (stack + constant)->name.start, (stack + constant)->name.length, offset);
        break;
    case GET_INDEX_OP:
        constant = bytecode->code[offset + 1];
        offset = WriteLocalInstruction("GET_INDEX_OP", buffer, bufferSize, constant, (stack + constant)->name.start, (stack + constant)->name.length, offset);
        break;
    default:
        offset = WriteInstructionWithoutConstant("Unknown Opcode", buffer, bufferSize, offset);
        break;
    }

    return offset;
}

static int DisassembleBytecodeInstruction(Compiler* compiler, IOSettings* settings, Local* stack, CompilerUpvalue* upvalues, Bytecode* bytecode, char* buffer, int bufferSize, int offset) {
    strcat_s(buffer, bufferSize, "\t");
    WriteInstructionAddress(buffer, bufferSize, offset);

    strcat_s(buffer, bufferSize, "\t\t");
    WriteLineNumber(bytecode, buffer, bufferSize, offset);

    strcat_s(buffer, bufferSize, "\t  ");
    offset = WriteBytecodeInstruction(compiler, settings, stack, upvalues, bytecode, buffer, bufferSize, offset);

    return offset;
}

static void WriteBytecodeLineDivider(char* buffer, int bufferSize) {
    strcat_s(buffer, bufferSize, "\n------------------------------------------------------------------------------------------\n");
}

void DisassembleBytecode(Compiler* compiler, Local* stack, CompilerUpvalue* upvalues, Bytecode* bytecode, const char* functionName, IOSettings* bytecodeSettings) {
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
        offset = DisassembleBytecodeInstruction(compiler, bytecodeSettings, stack, upvalues, bytecode, buffer, bufferSize, offset);
        strcat_s(buffer, bufferSize, "\n");
    }

    WriteBytecodeLineDivider(buffer, bufferSize);
    strcat_s(buffer, bufferSize, "\n");
    HandleOutput(buffer, bytecodeSettings);
}