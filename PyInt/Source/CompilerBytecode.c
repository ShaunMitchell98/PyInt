#include "../Headers/Compiler.h"
#include "../Headers/Bytecode.h"
#include "../Headers/CompilerBytecode.h"
#include "../Headers/CompilerErrors.h"

void WriteByte(Compiler* compiler, uint8_t byte) {
    WriteBytecode(&compiler->function->bytecode, byte, compiler->parser->previous.line);
}

void WriteBytes(Compiler* compiler, uint8_t byte1, uint8_t byte2) {
    WriteByte(compiler, byte1);
    WriteByte(compiler, byte2);
}

void WriteReturn(Compiler* compiler) {
    WriteBytes(compiler, CONSTANT_OP, StoreInBytecodeValueArray(compiler, NONE_VAL));
    WriteByte(compiler, RETURN_OP);
}

uint8_t StoreInBytecodeValueArray(Compiler* compiler, Value value) {
    int bytecodeValueArrayAddress = AddConstantToValueArray(&compiler->function->bytecode, value);
    if (bytecodeValueArrayAddress > UINT8_MAX) {
        Error("Too many constants stored in bytecode constants");
        return 0;
    }
    
    return (uint8_t) bytecodeValueArrayAddress;
}

void WriteConstantOperation(Compiler* compiler, Value value) {
    WriteBytes(compiler, CONSTANT_OP, StoreInBytecodeValueArray(compiler, value));
}

int WriteJump(Compiler* compiler, uint8_t opcode) {
    WriteByte(compiler, opcode);
    WriteByte(compiler, 0xff);
    WriteByte(compiler, 0xff);
    return compiler->function->bytecode.count-2;
}

void WriteLoop(Compiler* compiler, int loopStart) {
    WriteByte(compiler, LOOP_OP);
    
    int offset = compiler->function->bytecode.count-loopStart+2;
    
    if (offset > UINT16_MAX) {
        Error("Loop body too large");
    }
    
    WriteByte(compiler, (offset >> 8) & 0xff);
    WriteByte(compiler, offset & 0xff);
}

void PatchJump(Compiler* compiler, int offset) {
    int jump = compiler->function->bytecode.count-offset-2;
    
    if (jump > UINT16_MAX) {
        Error("Too much code to jump over");
    }
    
    compiler->function->bytecode.code[offset] = (jump >> 8) * 0xff;
    compiler->function->bytecode.code[offset+1] = jump & 0xff;
}
