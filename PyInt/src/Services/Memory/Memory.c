#include <stdlib.h>

#include "../../Headers/Common.h"
#include "Memory.h"
#include "../../Types/Closure/Closure.h"
#include "../../Types/Function/Function.h"
#include "../../Types/String/String.h"
#include "../../Types/Upvalue/Upvalue.h"
#include "../../Types/NativeFunction/NativeFunction.h"
#include "../Bytecode/BytecodeFunctions.h"
#include "../Disassembly/GarbageCollectionDisassembly/GarbageCollectionDisassembly.h"
#include "../GarbageCollection/GarbageCollection.h"
#include "../../Virtual Machine/VM/VM.h"

void* Reallocate(GarbageCollector* garbageCollector, void* currentArray, size_t oldSize, size_t newSize) {
    garbageCollector->bytesAllocated += newSize - oldSize;

    if (newSize > oldSize) {
        if (garbageCollector->bytesAllocated > garbageCollector->thresholdForCollection) {
            CollectGarbage(garbageCollector);
        }
    }

    if (newSize == 0) {
		free(currentArray);
		return NULL;
	}
	return realloc(currentArray, newSize);
}

void FreeObject(GarbageCollector* garbageCollector, Object* object) {
    WriteObjectDeallocation(garbageCollector->garbageSettings, object);
    switch(object->type) {
        case CLOSURE: {
            Closure* closure = (Closure*)object;
            FREE_ARRAY(garbageCollector, Upvalue*, closure->upvalues, closure->upvalueCount);
            FREE(garbageCollector, Closure, object);
            break;
        }
        case FUNCTION: {
            Function* function = (Function*)object;
            FreeBytecode(garbageCollector, &function->bytecode);
            FREE(garbageCollector, Function, object);
            break;
        }
        case NATIVE: {
            FREE(garbageCollector, NativeFunction, object);
            break;
        }
        case STRING: {
            String* string = (String*)object;
            FREE_ARRAY(garbageCollector, char, string->chars, string->length+1);
            FREE(garbageCollector, String, object);
            break;
        }
        case UPVALUE: {
            FREE(garbageCollector, Upvalue, object);
            break;
        }
    }
}

void FreeObjects(GarbageCollector* garbageCollector) {
    Object* object = garbageCollector->heap;
    while (object != NULL) {
        Object* next = (Object*) object->next;
        FreeObject(garbageCollector, object);
        object = next;
    }

    free(garbageCollector->greyObjectsStack);
}