#include "CompilerBytecode.h"
#include "../Errors/Errors.h"
#include "../../Services/Bytecode/BytecodeFunctions.h"


void WriteByte(Bytecode* bytecode, Services* services, uint8_t byte) {
    WriteBytecode(services->garbageCollector, bytecode, byte, services->parser->previous.line);
}

void WriteBytes(Bytecode* bytecode, Services* services, uint8_t byte1, uint8_t byte2) {
    WriteByte(bytecode, services, byte1);
    WriteByte(bytecode, services, byte2);
}

void WriteReturn(FunctionType type, Bytecode* bytecode, Services* services) {
    if (type == INITIALISER_FUNCTION) {
        WriteBytes(bytecode, services, GET_LOCAL_OP, 0);
    }
    else {
        WriteBytes(bytecode, services, CONSTANT_OP, StoreInBytecodeConstantsTable(bytecode, services, NONE_VAL));
    }
    WriteByte(bytecode, services, RETURN_OP);
}

uint8_t StoreInBytecodeConstantsTable(Bytecode* bytecode, Services* services, Value value) {
    int bytecodeValueArrayAddress = AddConstantToValueArray(services->garbageCollector, bytecode, value);
    if (bytecodeValueArrayAddress > UINT8_MAX) {
        Error("Too many constants stored in bytecode constants");
        return 0;
    }
    
    return (uint8_t) bytecodeValueArrayAddress;
}

void WriteConstantOperation(Bytecode* bytecode, Services* services, Value value) {
    WriteBytes(bytecode, services, CONSTANT_OP, StoreInBytecodeConstantsTable(bytecode, services, value));
}

int WriteJump(Bytecode* bytecode, Services* services, uint8_t opcode) {
    WriteByte(bytecode, services, opcode);
    WriteByte(bytecode, services, 0xff);
    WriteByte(bytecode, services, 0xff);
    return bytecode->count-2;
}

void WriteLoop(Bytecode* bytecode, Services* services, int loopStart) {
    WriteByte(bytecode, services, LOOP_OP);
    
    int offset = bytecode->count-loopStart+2;
    
    if (offset > UINT16_MAX) {
        Error("Loop body too large");
    }
    
    WriteByte(bytecode, services, (offset >> 8) & 0xff);
    WriteByte(bytecode, services, offset & 0xff);
}

void PatchJump(Bytecode* bytecode, int offset) {
    int jump = bytecode->count-offset-2;
    
    if (jump > UINT16_MAX) {
        Error("Too much code to jump over");
    }
    
    bytecode->code[offset] = (jump >> 8) * 0xff;
    bytecode->code[offset+1] = jump & 0xff;
}
