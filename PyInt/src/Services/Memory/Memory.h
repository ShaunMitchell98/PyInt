#ifndef PyInt_Memory_h
#define PyInt_Memory_h

#include "../../Headers/Common.h"
#include "../../Types/Object/Object.h"
#include "../../Virtual Machine/Settings/Settings.h"
#include "../GarbageCollection/GarbageCollector/GarbageCollector.h"

#define ALLOCATE(collector, type, count) \
    (type*)Reallocate(collector, NULL, 0, sizeof(type)*(count))

#define GROW_CAPACITY(capacity) \
	((capacity) < 8 ? 8 : (capacity) * 2)

#define FREE(collector, type, pointer) \
	Reallocate(collector, pointer, sizeof(type), 0)

#define GROW_ARRAY(collector, currentArray, type, oldCount, newCount) (type*) Reallocate(collector, currentArray, sizeof(type) * (oldCount), \
		sizeof(type) * (newCount))

#define FREE_ARRAY(collector, type, pointer, oldCount) \
	Reallocate(collector, pointer, sizeof(type) * (oldCount), 0)

void* Reallocate(GarbageCollector* garbageCollector, void* currentArray, size_t oldSize, size_t newSize);
void FreeObject(GarbageCollector* garbageCollector, Object* object);
void FreeObjects(GarbageCollector* garbageCollector);
#endif
