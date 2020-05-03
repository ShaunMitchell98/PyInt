#include <string.h>

#include "NativeFunction.h"

NativeFunction* NewNativeFunction(Object* heap, NativeFn function, const char* name) {
    NativeFunction* native = (NativeFunction*)AllocateObject(heap, sizeof(NativeFunction), NATIVE);
    native->function = function;
    native->name = name;
    return native;
}

void PrintNativeFunction(const char* name, char* buffer, int bufferSize) {
    strcat_s(buffer, bufferSize, "<fn ");
    strcat_s(buffer, bufferSize, name);
    strcat_s(buffer, bufferSize, ">");
};
