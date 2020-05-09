#include "GarbageCollector.h"
#include "../../../Types/Value/ValueFunctions.h"

void InitGarbageCollector(GarbageCollector* garbageCollector, IOSettings* settings) {
	garbageCollector->garbageSettings = settings;
	garbageCollector->stackTop = garbageCollector->stack;
	garbageCollector->callFrames = NULL;
	garbageCollector->callFrameCount = 0;
	garbageCollector->openUpvalues = NULL;
	garbageCollector->greyObjectsStack = NULL;
	garbageCollector->greyObjectsCount = 0;
	garbageCollector->greyObjectsCapacity = 0;
	garbageCollector->compiler = NULL;
	garbageCollector->heap = NULL;
	garbageCollector->bytesAllocated = 0;
	garbageCollector->thresholdForCollection = 1024 * 1024;
	InitValueArray(&garbageCollector->unallocatedTemporaries);
}

void AddTemporary(GarbageCollector* garbageCollector, Value value) {
	ValueArray* array = &garbageCollector->unallocatedTemporaries;

	if (array->capacity < array->count + 1) {
		int oldCapacity = array->capacity;
		array->capacity = GROW_CAPACITY(array->capacity);
		array->values = GROW_ARRAY(array->values, Value, array->capacity);
	}
	array->values[array->count] = value;
	array->count++;
}

void FreeTemporaries(GarbageCollector* garbageCollector) {
	FreeValueArray(garbageCollector, &garbageCollector->unallocatedTemporaries);
}