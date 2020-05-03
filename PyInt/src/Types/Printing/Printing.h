#ifndef PyInt_Types_Printing_h
#define PyInt_Types_Printing_h

#include "../Value/Value.h"
#include "../../Headers/PrintType.h"
#include "../../Virtual Machine/Settings/Settings.h"

void PrintObject(IOSettings* settings, Value value, PrintType printType, char* buffer, int bufferSize);

#endif
