#ifndef PyInt_GarbageCollector_h
#define PyInt_GarbageCollector_h

#include <stdlib.h>

#include "../../../Types/Value/Value.h"
#include "../../../Types/Object/Object.h"
#include "../../../Types/Upvalue/Upvalue.h"
#include "../../Table/Table.h"
#include "../../../Virtual Machine/Settings/Settings.h"
#include "../../../Virtual Machine/CallFrame/CallFrame.h"
#include "../../../Compilation/Compiler/Compiler.h"

#define GROW_CAPACITY(capacity) \
	((capacity) < 8 ? 8 : (capacity) * 2)

#define GROW_ARRAY(currentArray, type, newCount) \
	(type*) realloc(currentArray, sizeof(type) * (newCount))

typedef struct {
	IOSettings* garbageSettings;
	Table* globalsTable;
	Table* stringsTable;
	Value* stack;
	Value* stackTop;
	CallFrame* callFrames;
	int callFrameCount;
	Upvalue* openUpvalues;
	Compiler* compiler;
	Object** greyObjectsStack;
	int greyObjectsCount;
	int greyObjectsCapacity;
	Object* heap;
	size_t bytesAllocated;
	size_t thresholdForCollection;
	ValueArray unallocatedTemporaries;
	String* initString;
} GarbageCollector;

void InitGarbageCollector(GarbageCollector* garbageCollector, IOSettings* settings);
void AddTemporary(GarbageCollector* garbageCollector, Value value);
void FreeTemporaries(GarbageCollector* garbageCollector);

#endif