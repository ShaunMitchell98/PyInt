#ifndef PyInt_Closure_h
#define PyInt_Closure_h

#include "../Object/Object.h"
#include "../Function/Function.h"
#include "../Upvalue/Upvalue.h"
#include "../Value/Value.h"
#include "../../Virtual Machine/Settings/Settings.h"

typedef struct {
    Object obj;
    Function* function;
    Upvalue** upvalues;
    int upvalueCount;
} Closure;

#define IS_CLOSURE(value) IsObjType(value, CLOSURE)
#define AS_CLOSURE(value) ((Closure*)AS_OBJ(value))

#endif
