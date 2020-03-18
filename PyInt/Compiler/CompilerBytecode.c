#include "Compiler.h"
#include "Bytecode.h"
#include "CompilerBytecode.h"
#include "CompilerErrors.h"

void WriteByte(uint8_t byte) {
    WriteBytecode(&currentCompiler->function->bytecode, byte, parser.previous.line);
}

void WriteBytes(uint8_t byte1, uint8_t byte2) {
    WriteByte(byte1);
    WriteByte(byte2);
}

void WriteReturn() {
    WriteByte(RETURN_OP);
}

uint8_t StoreConstant(Value value) {
    int address = AddConstant(&currentCompiler->function->bytecode, value);
    if (address > UINT8_MAX) {
        Error("Too many constants stored in bytecode constants");
        return 0;
    }
    
    return (uint8_t) address;
}

void WriteConstantOperation(Value value) {
    WriteBytes(CONSTANT_OP, StoreConstant(value));
}

int WriteJump(uint8_t opcode) {
    WriteByte(opcode);
    WriteByte(0xff);
    WriteByte(0xff);
    return currentCompiler->function->bytecode.count-2;
}

void WriteLoop(int loopStart) {
    WriteByte(LOOP_OP);
    
    int offset = currentCompiler->function->bytecode.count-loopStart+2;
    
    if (offset > UINT16_MAX) {
        Error("Loop body too large");
    }
    
    WriteByte((offset >> 8) & 0xff);
    WriteByte(offset & 0xff);
}

void PatchJump(int offset) {
    int jump = currentCompiler->function->bytecode.count-offset-2;
    
    if (jump > UINT16_MAX) {
        Error("Too much code to jump over");
    }
    
    currentCompiler->function->bytecode.code[offset] = (jump >> 8) * 0xff;
    currentCompiler->function->bytecode.code[offset+1] = jump & 0xff;
}
