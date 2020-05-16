#ifndef PyInt_ClassFunctions_h
#define PyInt_ClassFunctions_h

#include "Class.h"
#include "../../Services/GarbageCollection/GarbageCollector/GarbageCollector.h"

Class* NewClass(GarbageCollector* garbageCollector, String* name);

#endif