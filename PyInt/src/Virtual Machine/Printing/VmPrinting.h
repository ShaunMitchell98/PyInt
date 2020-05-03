#ifndef PyInt_VmPrinting_h
#define PyInt_VmPrinting_h

#include "../Settings/Settings.h"
#include "../../Types/Value/Value.h"

bool PrintValueToTerminal(IOSettings* settings, Value value);
bool PrintValueToFile(IOSettings* settings, Value value);
bool PrintValueToString(IOSettings* settings, Value value);

#endif