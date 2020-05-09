#ifndef PyInt_GarbageCollection_h
#define PyInt_GarbageCollection_h

#include "GarbageCollector/GarbageCollector.h"
#include "../../Types/Object/Object.h"
#include "../../Virtual Machine/Settings/Settings.h"

#define GC_HEAP_GROW_FACTOR 2

void InitialiseGarbageCollector(GarbageCollector* garbageCollector);
void MarkObject(GarbageCollector* garbageCollector, Object* object);
void MarkValue(GarbageCollector* garbageCollector, Value value);
void CollectGarbage(GarbageCollector* garbageCollector);

#endif