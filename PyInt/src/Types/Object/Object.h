#ifndef PyInt_Object_h
#define PyInt_Object_h

#include "../../Headers/Common.h"

typedef enum {
    CLOSURE,
    FUNCTION,
    NATIVE,
    STRING,
    UPVALUE
} ObjType;

typedef struct {
    ObjType type;
    struct Object* next;
} Object;

Object* AllocateObject(Object* heap, size_t size, ObjType type);

#endif
