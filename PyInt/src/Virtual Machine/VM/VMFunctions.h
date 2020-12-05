#ifndef PyInt_VM_Functions_h
#define PyInt_VM_Functions_h

#include "../Settings/Settings.h"

typedef enum {
    INTERPRET_COMPILE_ERROR,
    INTERPRET_RUNTIME_ERROR,
    INTERPRET_OK
} InterpretResult;

InterpretResult Interpret(const char* sourceCode, Settings* settings);

#endif

