#ifndef PyInt_ClosureFunctions_h
#define PyInt_ClosureFunctions_h

#include "Closure.h"
#include "../../Services/GarbageCollection/GarbageCollector/GarbageCollector.h"

Closure* NewClosure(GarbageCollector* garbageCollector, Function* function);

#endif