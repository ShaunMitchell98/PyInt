#ifndef PyInt_Compiler_h
#define PyInt_Compiler_h

#include "../../Types/Function/Function.h"
#include "../Variables/Upvalue/CompilerUpvalue.h"
#include "../Variables/Local/Local.h"

typedef struct {
    struct Compiler* enclosing;
    Function* function;
    Local locals[UINT8_COUNT];
    int localCount;
    CompilerUpvalue upvalues[UINT8_COUNT];
} Compiler;

#endif
