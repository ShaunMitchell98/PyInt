#ifndef PyInt_NativeFunction_h
#define PyInt_NativeFunction_h

#include "../Object/Object.h"
#include "../../Virtual Machine/Settings/Settings.h"
#include "../Value/Value.h"

typedef Value(*NativeFn)(IOSettings* settings, int argCount, Value* args);

typedef struct {
    Object obj;
    const char* name;
    NativeFn function;
} NativeFunction;

#define IS_NATIVE(value) IsObjType(value, NATIVE)
#define AS_NATIVE(value) (((NativeFunction*)AS_OBJ(value))->function)

NativeFunction* NewNativeFunction(Object* heap, NativeFn function, const char* name);
void PrintNativeFunction(const char* name, char* buffer, int bufferSize);

#endif
