#ifndef PyInt_ObjectFunctions_h
#define PyInt_ObjectFunctions_h

#include "Object.h"
#include "../../Services/GarbageCollection/GarbageCollector/GarbageCollector.h"

Object* AllocateObject(GarbageCollector* garbageCollector, size_t size, ObjType type);
#endif