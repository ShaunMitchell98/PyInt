#ifndef PyInt_NativeFunctionFunctions_h
#define PyInt_NativeFunctionFunctions_h

#include "NativeFunction.h"
#include "../../Services/GarbageCollection/GarbageCollector/GarbageCollector.h"

NativeFunction* NewNativeFunction(GarbageCollector* garbageCollector, NativeFn function, const char* name);
void PrintNativeFunction(const char* name, char* buffer, int bufferSize);

#endif