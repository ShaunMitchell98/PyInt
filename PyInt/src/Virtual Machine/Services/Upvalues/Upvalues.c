#include "Upvalues.h"

Upvalue* CaptureUpvalue(VM* vm, Value* local) {
    Upvalue* prevUpvalue = NULL;
    Upvalue* upvalue = vm->openUpvalues;

    while (upvalue != NULL && upvalue->location > local) {
        prevUpvalue = upvalue;
        upvalue = (Upvalue*)upvalue->next;
    }

    if (upvalue != NULL && upvalue->location == local) return upvalue;

    Upvalue* createdUpvalue = NewUpvalue(vm->heap, local);
    createdUpvalue->next = (struct Upvalue*) upvalue;

    if (prevUpvalue == NULL) {
        vm->openUpvalues = createdUpvalue;
    }
    else {
        prevUpvalue->next = (struct Upvalue*) createdUpvalue;
    }

    return createdUpvalue;
}

void CloseUpvalues(VM* vm, Value* last) {
    while (vm->openUpvalues != NULL && vm->openUpvalues->location >= last) {
        Upvalue* upvalue = vm->openUpvalues;
        upvalue->closed = *upvalue->location;
        upvalue->location = &upvalue->closed;
        vm->openUpvalues = (Upvalue*)upvalue->next;
    }
}