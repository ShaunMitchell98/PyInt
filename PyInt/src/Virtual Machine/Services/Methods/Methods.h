#ifndef PyInt_Methods_h
#define PyInt_Methods_h

#include "../../../Types/String/String.h"
#include "../../VM/VM.h"
#include "../../../Types/Class/Class.h"

bool GetMethod(VM* vm, Class* klass, String* name);
bool Invoke(VM* vm, String* name, int argCount);

#endif