#ifndef PyInt_ValueFunctions_h
#define PyInt_ValueFunctions_h

#include "Value.h"
#include "../../Services/GarbageCollection/GarbageCollector/GarbageCollector.h"

void InitValueArray(ValueArray* array);
void WriteValueArray(GarbageCollector* garbageCollector, ValueArray* array, Value value);
void FreeValueArray(GarbageCollector* garbageCollector, ValueArray* array);
void WriteValue(IOSettings* settings, Value value, PrintType printType, char* buffer, int bufferSize);

#endif