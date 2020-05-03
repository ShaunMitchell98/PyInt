#ifndef PyInt_Upvalues_h
#define PyInt_Upvalues_h

#include "../../../Types/Upvalue/Upvalue.h"
#include "../../VM/VM.h"
#include "../../../Types/Value/Value.h"

Upvalue* CaptureUpvalue(VM* vm, Value* local);
void CloseUpvalues(VM* vm, Value* last);

#endif