#ifndef PyInt_Function_h
#define PyInt_Function_h

#include "../../Headers/Common.h"
#include "../Object/Object.h"
#include "../../Services/Bytecode/Bytecode.h"
#include "../String/String.h"
#include "../Value/Value.h"

typedef enum {
    TYPE_FUNCTION,
    TYPE_SCRIPT
} FunctionType;

typedef struct {
    Object obj;
    int arity;
    int upvalueCount;
    bool hasReturnStatement;
    Bytecode bytecode;
    String* name;
} Function;

#define IS_FUNCTION(value) IsObjType(value, FUNCTION)
#define AS_FUNCTION(value) ((Function*)AS_OBJ(value))

#endif