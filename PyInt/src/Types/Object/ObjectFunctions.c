#include "ObjectFunctions.h"
#include "Object.h"
#include "../../Services/Memory/Memory.h"
#include "../../Services/Disassembly/GarbageCollectionDisassembly/GarbageCollectionDisassembly.h"

Object* AllocateObject(GarbageCollector* garbageCollector, size_t size, ObjType type) {
    Object* object = (Object*)Reallocate(garbageCollector, NULL, 0, size);
    object->type = type;
    object->isReachable = false;
    object->next = (struct Object*) garbageCollector->heap;
    garbageCollector->heap = object;
    WriteObjectAllocation(garbageCollector->garbageSettings, object, size);

    return object;
}
