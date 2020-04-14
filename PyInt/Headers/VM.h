#ifndef PyInt_VM_h
#define PyInt_VM_h

#define FRAMES_MAX 64
#define STACK_MAX (FRAMES_MAX * UINT8_COUNT)

#include "Common.h"
#include "Value.h"
#include "Bytecode.h"
#include "HashTable.h"
#include "InterpreterSettings.h"
#include "VmPrint.h"

typedef struct {
    ObjFunction* function;
    uint8_t* ip;
    Value* locals;
} CallFrame;

struct sVM {
    CallFrame frames[FRAMES_MAX];
    int frameCount;
    Value stack[STACK_MAX];
    Value* stackTop;
    Obj* heap;
    Table strings;
    Table globals;
    int arrayIndex;
    InterpreterSettings settings;
};

typedef enum {
    INTERPRET_COMPILE_ERROR,
    INTERPRET_RUNTIME_ERROR,
    INTERPRET_OK
} InterpretResult;

void Push(VM* vm, Value value);
Value Pop(VM* vm);
void InitVM(VM* vm, InterpreterSettings settings);
InterpretResult Interpret(const char* sourceCode, InterpreterSettings settings);
void FreeVM(VM* vm);

#endif
