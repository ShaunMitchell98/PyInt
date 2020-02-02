#ifndef PyInt_memory_h
#define PyInt_memory_h

#include <stdlib.h>

#include "Common.h"
#include "Memory.h"

void* Reallocate(void* currentArray, size_t oldSize, size_t newSize) {
	if (newSize == 0) {
		free(currentArray);
		return NULL;
	}
	return realloc(currentArray, newSize);
}
#endif
