#ifndef PyInt_Stack_h
#define PyInt_Stack_h

#include "../../../Types/Value/Value.h"
#include "../../VM/VM.h"

void Push(VM* vm, Value value);
Value Pop(VM* vm);
Value Peek(VM* vm, int i);
void ResetStack(VM* vm);

#endif