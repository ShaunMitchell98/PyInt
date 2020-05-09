#include "ClosureFunctions.h"
#include "../../Services/Memory/Memory.h"
#include "../Object/ObjectFunctions.h"

Closure* NewClosure(GarbageCollector* garbageCollector, Function* function) {
    Upvalue** upvalues = ALLOCATE(garbageCollector, Upvalue*, function->upvalueCount);
    for (int i = 0; i < function->upvalueCount; i++) {
        upvalues[i] = NULL;
    }

    Closure* closure = (Closure*)AllocateObject(garbageCollector, sizeof(Closure), CLOSURE);
    closure->function = function;
    closure->upvalues = upvalues;
    closure->upvalueCount = function->upvalueCount;
    return closure;
}