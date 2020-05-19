#ifndef PyInt_Call_h
#define PyInt_Call_h

#include "../../VM/VM.h"
#include "../../../Types/Value/Value.h"
#include "../../../Headers/Common.h"
#include "../../../Types/Closure/Closure.h"

bool Call(VM* vm, Closure* closure, int argCount);
bool CallValue(VM* vm, Value callee, int argCount);

#endif