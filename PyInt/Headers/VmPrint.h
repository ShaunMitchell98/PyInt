#ifndef PyInt_VmPrint_h
#define PyInt_VmPrint_h

#include "Value.h"

typedef struct sVM VM;

bool PrintValueToTerminal(VM* vm, Value a);
bool PrintValueToFile(VM* vm, Value a);
bool PrintValueToString(VM* vm, Value a);

#endif