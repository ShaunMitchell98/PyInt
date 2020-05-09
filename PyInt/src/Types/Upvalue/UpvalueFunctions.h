#ifndef PyInt_UpvalueFunctions_h
#define PyInt_UpvalueFunctions_h

#include "Upvalue.h"
#include "../../Services/GarbageCollection/GarbageCollector/GarbageCollector.h"

Upvalue* NewUpvalue(GarbageCollector* garbageCollector, Value* slot);

#endif