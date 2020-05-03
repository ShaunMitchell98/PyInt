#include "Closure.h"
#include "../../Services/Memory/Memory.h"

Closure* NewClosure(Object* heap, Function* function) {
    Upvalue** upvalues = ALLOCATE(Upvalue*, function->upvalueCount);
    for (int i = 0; i < function->upvalueCount; i++) {
        upvalues[i] = NULL;
    }

    Closure* closure = (Closure*)AllocateObject(heap, sizeof(Closure), CLOSURE);
    closure->function = function;
    closure->upvalues = upvalues;
    closure->upvalueCount = function->upvalueCount;
    return closure;
}