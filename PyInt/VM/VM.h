#ifndef PyInt_VM_h
#define PyInt_VM_h

#define STACK_MAX 256

#include "Common.h"
#include "Value.h"
#include "Bytecode.h"
#include "HashTable.h"

typedef struct {
    Value stack[STACK_MAX];
    uint8_t* ip;
    Bytecode* bytecode;
    Value* stackTop;
    Obj* heap;
    Table strings;
    Table globals;
    int arrayIndex;
} VM;

typedef enum {
    INTERPRET_COMPILE_ERROR,
    INTERPRET_RUNTIME_ERROR,
    INTERPRET_OK
} InterpretResult;

extern VM vm;

void InitVM(void);
InterpretResult Interpret(const char* sourceCode, const char* path);
void FreeVM(void);

#endif
