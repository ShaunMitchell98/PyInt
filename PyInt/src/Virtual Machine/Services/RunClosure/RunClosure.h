#ifndef PyInt_RunClosure_h
#define PyInt_RunClosure_h

#include "../../VM/VM.h"
#include "../../../Types/Closure/Closure.h"
#include "../../../Headers/Common.h"

bool RunClosure(VM* vm, Closure* closure, int argCount);

#endif