#include "Object.h"
#include "../../Services/Memory/Memory.h"

Object* AllocateObject(Object* heap, size_t size, ObjType type) {
    Object* object = (Object*)Reallocate(NULL, 0, size);
    object->type = type;
    object->next = (struct Object*) heap;
    heap = object;
    return object;
}
