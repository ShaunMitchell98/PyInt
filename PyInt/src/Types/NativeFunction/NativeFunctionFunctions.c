#include <string.h>

#include "NativeFunctionFunctions.h"
#include "../Object/ObjectFunctions.h"

NativeFunction* NewNativeFunction(GarbageCollector* garbageCollector, NativeFn function, const char* name) {
    NativeFunction* native = (NativeFunction*)AllocateObject(garbageCollector, sizeof(NativeFunction), NATIVE);
    native->function = function;
    native->name = name;
    return native;
}

void PrintNativeFunction(const char* name, char* buffer, int bufferSize) {
    strcat_s(buffer, bufferSize, "<fn ");
    strcat_s(buffer, bufferSize, name);
    strcat_s(buffer, bufferSize, ">");
};
