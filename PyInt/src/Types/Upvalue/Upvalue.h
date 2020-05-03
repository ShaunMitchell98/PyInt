#ifndef PyInt_Upvalue_h
#define PyInt_Upvalue_h

#include "../Object/Object.h"
#include "../Value/Value.h"

typedef struct {
    Object obj;
    Value* location;
    Value closed;
    struct Upvalue* next;
} Upvalue;

Upvalue* NewUpvalue(Object* heap, Value* slot);
#endif
