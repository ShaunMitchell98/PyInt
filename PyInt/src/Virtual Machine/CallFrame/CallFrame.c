#include "CallFrame.h"

uint8_t ReadByte(CallFrame* frame) {
    frame->ip++;
    return *(frame->ip - 1);
}

Value ReadConstant(CallFrame* frame) {
    int address = ReadByte(frame);
    return frame->closure->function->bytecode.constants.values[address];
}

String* ReadString(CallFrame* frame) {
    int address = ReadByte(frame);
    return AS_STRING(frame->closure->function->bytecode.constants.values[address]);
}