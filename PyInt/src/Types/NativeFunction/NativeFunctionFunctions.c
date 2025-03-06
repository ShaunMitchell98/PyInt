#include <string.h>

// Include our platform compatibility header
#include "platform_compat.h"

#include "NativeFunctionFunctions.h"
#include "../Object/ObjectFunctions.h"

NativeFunction* NewNativeFunction(GarbageCollector* garbageCollector, NativeFn function, const char* name) {
    NativeFunction* native = (NativeFunction*)AllocateObject(garbageCollector, sizeof(NativeFunction), NATIVE);
    native->function = function;
    native->name = name;
    return native;
}

void PrintNativeFunction(const char* name, char* buffer, int bufferSize) {
    PYINT_STRCAT(buffer, bufferSize, "<fn ");
    PYINT_STRCAT(buffer, bufferSize, name);
    PYINT_STRCAT(buffer, bufferSize, ">");
};
