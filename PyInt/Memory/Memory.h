#ifndef PyInt_Memory_h
#define PyInt_Memory_h

#include "Common.h"
#include "Object.h"

#define ALLOCATE(type, count) \
    (type*)Reallocate(NULL, 0, sizeof(type)*(count))

#define GROW_CAPACITY(capacity) \
	((capacity) < 8 ? 8 : (capacity) * 2)

#define FREE(type, pointer) \
	Reallocate(pointer, sizeof(type), 0)

#define GROW_ARRAY(currentArray, type, oldCount, newCount) (type*) Reallocate(currentArray, sizeof(type) * (oldCount), \
		sizeof(type) * (newCount))

#define FREE_ARRAY(type, pointer, oldCount) \
	Reallocate(pointer, sizeof(type) * (oldCount), 0)

void* Reallocate(void* currentArray, size_t oldSize, size_t newSize);
void FreeObject(Obj* obj);
#endif
