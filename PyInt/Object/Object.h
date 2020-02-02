#ifndef PyInt_Object_h
#define PyInt_Object_h

#include "Common.h"
#include "Value.h"

#define OBJ_TYPE(value) (AS_OBJ(value)->type)

#define AS_STRING(value) ((ObjString*)AS_OBJ(value))
#define AS_CSTRING(value) (((ObjString*)AS_OBJ(value))->chars)

typedef enum {
    OBJ_STRING
} ObjType;

struct sObj {
    ObjType type;
    struct sObj* next;
};

struct sObjString {
    Obj obj;
    int length;
    char* chars;
    uint32_t hash;
};

ObjString* TakeString(char* chars, int length);
ObjString* CopyString(const char* chars, int length);
void PrintObject(Value value, bool tab);

#endif
