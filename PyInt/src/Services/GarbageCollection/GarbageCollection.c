#include <stdlib.h>

#include "GarbageCollection.h"
#include "../Disassembly/GarbageCollectionDisassembly/GarbageCollectionDisassembly.h"
#include "../../Compilation/Compiler/CompilerFunctions.h"
#include "../Memory/Memory.h"
#include "../../Types/Function/Function.h"
#include "../../Types/BoundMethod/BoundMethod.h"
#include "../../Types/Class/Class.h"
#include "../../Types/ClassInstance/ClassInstance.h"
#include "../Table/TableFunctions.h"

static void MarkCallFrames(GarbageCollector* garbageCollector, CallFrame* callFrames, int frameCount) {
    for (int i = 0; i < frameCount; i++) {
        MarkObject(garbageCollector, (Object*)(callFrames + i)->closure);
    }
}

static void MarkTable(GarbageCollector* garbageCollector, Table* table) {
    for (int i = 0; i < table->capacity; i++) {
        Entry* entry = &table->entries[i];
        MarkObject(garbageCollector, (Object*)entry->key);
        MarkValue(garbageCollector, entry->value);
    }
}

static void MarkOpenUpvalues(GarbageCollector* garbageCollector, Upvalue* upvalues) {
    for (Upvalue* upvalue = upvalues; upvalue != NULL; upvalue = (struct Upvalue*)upvalue->next) {
        MarkObject(garbageCollector, (Object*)upvalue);
    }
}

static void MarkArray(GarbageCollector* garbageCollector, ValueArray* valueArray) {
    for (int i = 0; i < valueArray->count; i++) {
        MarkValue(garbageCollector, valueArray->values[i]);
    }
}

static void MarkCompilerRoots(GarbageCollector* garbageCollector) {
    Compiler* currentCompiler = garbageCollector->compiler;
    while (currentCompiler != NULL) {
        MarkObject(garbageCollector, (Object*)currentCompiler->function);
        currentCompiler = (struct Compiler*)currentCompiler->enclosing;
    }
}

static void MarkTemporaries(GarbageCollector* garbageCollector) {
    int count = garbageCollector->unallocatedTemporaries.count;
    for (int i = 0; i < count; i++) {
        MarkValue(garbageCollector, garbageCollector->unallocatedTemporaries.values[i]);
    }
}

static void BlackenObject(GarbageCollector* garbageCollector, Object* object) {
    WriteObjectBlackened(garbageCollector->garbageSettings, object);

    switch (object->type) {
        case BOUND_METHOD: {
            BoundMethod* boundMethod = (BoundMethod*)object;
            MarkValue(garbageCollector, boundMethod->receiver);
            MarkObject(garbageCollector, (Object*)boundMethod->method);
            break;
        }
        case CLASS_INSTANCE: {
            ClassInstance* instance = (ClassInstance*)object;
            MarkObject(garbageCollector, (Object*)instance->klass);
            MarkTable(garbageCollector, &instance->fields);
            break;
        }
        case CLASS: {
            Class* klass = (Class*)object;
            MarkObject(garbageCollector, (Object*)klass->name);
            MarkTable(garbageCollector, &klass->methods);
            Closure* closure = klass->init;
            MarkObject(garbageCollector, (Object*)closure->function);
            for (int i = 0; i < closure->upvalueCount; i++) {
                MarkObject(garbageCollector, (Object*)closure->upvalues[i]);
            }
            break;
        }
        case CLOSURE: {
            Closure* closure = (Closure*)object;
            MarkObject(garbageCollector, (Object*)closure->function);
            for (int i = 0; i < closure->upvalueCount; i++) {
                MarkObject(garbageCollector, (Object*)closure->upvalues[i]);
            }
            break;
        }
        case FUNCTION: {
            Function* function = (Function*)object;
            MarkObject(garbageCollector, (Object*)function->name);
            MarkArray(garbageCollector, &function->bytecode.constants);
        }
        case UPVALUE:
           MarkValue(garbageCollector, ((Upvalue*)object)->closed);
        case NATIVE:
        case STRING:
            break;
    }
}

static void TraceReferences(GarbageCollector* garbageCollector) {
    while (garbageCollector->greyObjectsCount > 0) {
        Object* object = garbageCollector->greyObjectsStack[--garbageCollector->greyObjectsCount];
        BlackenObject(garbageCollector, object);
    }
}

static void MarkRootObjects(GarbageCollector* garbageCollector) {

    MarkCompilerRoots(garbageCollector);
    MarkObject(garbageCollector, (Object*)garbageCollector->initString);
    MarkTemporaries(garbageCollector);

    for (Value* slot = garbageCollector->stack; slot < garbageCollector->stackTop; slot++) {
        MarkValue(garbageCollector, *slot);
    }

    MarkTable(garbageCollector, garbageCollector->globalsTable);

    MarkCallFrames(garbageCollector, garbageCollector->callFrames, garbageCollector->callFrameCount);
    MarkOpenUpvalues(garbageCollector, garbageCollector->openUpvalues);
}

static void RemoveWeakReferences(Table* table) {
    for (int i = 0; i < table->capacity; i++) {
        Entry* entry = &table->entries[i];
        if (entry->key != NULL && !entry->key->obj.isReachable) {
            DeleteTableEntry(table, entry->key);
        }
    }
}

static void Sweep(GarbageCollector* garbageCollector) {
    Object* previous = NULL;
    Object* current = garbageCollector->heap;

    while (current != NULL) {
        if (current->isReachable) {
            current->isReachable = false;
            previous = current;
            current = (struct Object*)current->next;
        }
        else {
            Object* unreached = current;

            current = (struct Object*)current->next;
            if (previous != NULL) {
                previous->next = (struct Object*)current;
            }
            else {
                garbageCollector->heap = current;
            }

            FreeObject(garbageCollector, unreached);
        }
    }
}

void InitialiseGarbageCollector(GarbageCollector* garbageCollector) {
    garbageCollector->greyObjectsStack = NULL;
    garbageCollector->greyObjectsCount = 0;
    garbageCollector->greyObjectsCapacity = 0;
}

void MarkObject(GarbageCollector* garbageCollector, Object* object) {
    if (object == NULL) {
        return;
    }
    if (object->isReachable) {
        return;
    }

    WriteObjectMarking(garbageCollector->garbageSettings, object);
    object->isReachable = true;

    if (garbageCollector->greyObjectsCapacity < garbageCollector->greyObjectsCount + 1) {
        garbageCollector->greyObjectsCapacity = GROW_CAPACITY(garbageCollector->greyObjectsCapacity);
        garbageCollector->greyObjectsStack = (Object**)realloc(garbageCollector->greyObjectsStack, sizeof(Object*) * garbageCollector->greyObjectsCapacity);
    }

    garbageCollector->greyObjectsStack[garbageCollector->greyObjectsCount++] = object;
}

void MarkValue(GarbageCollector* garbageCollector, Value value) {
    if (!IS_OBJ(value)) {
        return;
    }
    
    MarkObject(garbageCollector, AS_OBJ(value));
}
    
void CollectGarbage(GarbageCollector* garbageCollector) {
    size_t bytesBefore = garbageCollector->bytesAllocated;

    MarkRootObjects(garbageCollector);
    TraceReferences(garbageCollector);
    RemoveWeakReferences(garbageCollector->stringsTable);
    Sweep(garbageCollector);
    garbageCollector->thresholdForCollection = garbageCollector->bytesAllocated * GC_HEAP_GROW_FACTOR;
    WriteCollectionAmount(garbageCollector->garbageSettings, bytesBefore, garbageCollector->bytesAllocated);
}