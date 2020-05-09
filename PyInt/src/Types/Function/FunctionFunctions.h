#ifndef PyInt_FunctionFunctions_h
#define PyInt_FunctionFunctions_h

#include "Function.h"
#include "../../Headers/PrintType.h"
#include "../../Services/GarbageCollection/GarbageCollector/GarbageCollector.h"

Function* NewFunction(GarbageCollector* garbageCollector);
void PrintFunction(Function* function, char* buffer, int bufferSize, PrintType printType);

#endif