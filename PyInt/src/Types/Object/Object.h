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
    bool isReachable;
    struct Object* next;
} Object;

#endif
