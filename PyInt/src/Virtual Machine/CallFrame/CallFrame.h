#ifndef PyInt_CallFrame_h
#define PyInt_CallFrame_h

#include "../../Types/String/String.h"
#include "../../Types/Closure/Closure.h"
#include "../../Types/Value/Value.h"
#include "../../Headers/Common.h"

typedef struct {
    Closure* closure;
    uint8_t* ip;
    Value* locals;
} CallFrame;

uint8_t ReadByte(CallFrame* frame);
Value ReadConstant(CallFrame* frame);
String* ReadString(CallFrame* frame);

#endif