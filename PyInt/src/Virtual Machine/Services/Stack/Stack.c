#include "Stack.h"

void Push(VM* vm, Value value) {
    *vm->stackTop = value;
    vm->stackTop++;
    vm->garbageCollector->stackTop++;
}

Value Pop(VM* vm) {
    vm->stackTop--;
    vm->garbageCollector->stackTop--;
    return *vm->stackTop;
}

Value Peek(VM* vm, int i) {
    return *(vm->stackTop - i - 1);
}

void ResetStack(VM* vm) {
    vm->stackTop = vm->stack;
    vm->frameCount = 0;
}
