#include "Upvalue.h"
#include "../Value/Value.h"

Upvalue* NewUpvalue(Object* heap, Value* slot) {
    Upvalue* upvalue = (Upvalue*)AllocateObject(heap, sizeof(Upvalue), UPVALUE);
    upvalue->location = slot;
    upvalue->closed = NONE_VAL;
    upvalue->next = NULL;
    return upvalue;
}