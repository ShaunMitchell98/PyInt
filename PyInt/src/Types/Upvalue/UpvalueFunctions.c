#include "UpvalueFunctions.h"
#include "../Value/Value.h"
#include "../Object/ObjectFunctions.h"

Upvalue* NewUpvalue(GarbageCollector* garbageCollector, Value* slot) {
    Upvalue* upvalue = (Upvalue*)AllocateObject(garbageCollector, sizeof(Upvalue), UPVALUE);
    upvalue->location = slot;
    upvalue->closed = NONE_VAL;
    upvalue->next = NULL;
    return upvalue;
}