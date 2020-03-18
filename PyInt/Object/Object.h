#ifndef PyInt_Object_h
#define PyInt_Object_h

#include "Common.h"
#include "Value.h"
#include "Debug.h"

#define OBJ_TYPE(value) (AS_OBJ(value)->type)

#define AS_STRING(value) ((ObjString*)AS_OBJ(value))
#define AS_CSTRING(value) (((ObjString*)AS_OBJ(value))->chars)

#define IS_FUNCTION(value) IsObjType(value, OBJ_FUNCTION)
#define AS_FUNCTION(value) ((ObjFunction*)AS_OBJ(value))

typedef enum {
    OBJ_FUNCTION,
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

struct sObjFunction {
    Obj obj;
    int arity;
    Bytecode bytecode;
    ObjString* name;
};

typedef enum {
    TYPE_FUNCTION,
    TYPE_SCRIPT
} FunctionType;

ObjString* TakeString(char* chars, int length);
ObjString* CopyString(const char* chars, int length);
void PrintObject(Value value, PrintType printType);
ObjFunction* NewFunction(void);

static inline bool IsObjType(Value value, ObjType type) {
return IS_OBJ(value) && AS_OBJ(value)->type == type;
}
#endif
