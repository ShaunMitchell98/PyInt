#ifndef PyInt_RuntimeError_h
#define PyInt_RuntimeError_h

#include "../../VM/VM.h"

void RuntimeError(VM* vm, const char* format);

#endif