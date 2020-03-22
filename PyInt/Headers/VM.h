#ifndef PyInt_VM_h
#define PyInt_VM_h

#define FRAMES_MAX 64
#define STACK_MAX (FRAMES_MAX * UINT8_COUNT)

#include "Common.h"
#include "Value.h"
#include "Bytecode.h"
#include "HashTable.h"

typedef struct {
    ObjFunction* function;
    uint8_t* ip;
    Value* locals;
} CallFrame;

typedef struct {
    CallFrame frames [FRAMES_MAX];
    int frameCount;
    Value stack[STACK_MAX];
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
