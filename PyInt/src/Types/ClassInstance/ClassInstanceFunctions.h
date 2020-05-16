#ifndef PyInt_ClassInstanceFunctions_h
#define PyInt_ClassInstanceFunctions_h

#include "ClassInstance.h"
#include "../../Services/GarbageCollection/GarbageCollector/GarbageCollector.h"

ClassInstance* NewClassInstance(GarbageCollector* garbageCollector, Class* klass);

#endif