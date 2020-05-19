#ifndef PyInt_BoundMethodFunctions_h
#define PyInt_BoundMethodFunctions_h

#include "BoundMethod.h"
#include "../../Services/GarbageCollection/GarbageCollector/GarbageCollector.h"

BoundMethod* NewBoundMethod(GarbageCollector* garbageCollector, Value receiver, Closure* method);

#endif