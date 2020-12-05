#ifndef PyInt_VM_h
#define PyInt_VM_h

#define FRAMES_MAX 64
#define STACK_MAX (FRAMES_MAX * UINT8_COUNT)

#include "../../Headers/Common.h"
#include "../../Types/Closure/Closure.h"
#include "../../Types/Value/Value.h"
#include "../../Services/Table/Table.h"
#include "../Settings/Settings.h"
#include "../CallFrame/CallFrame.h"
#include "../../Types/Upvalue/Upvalue.h"
#include "../../Services/GarbageCollection/GarbageCollector/GarbageCollector.h"

typedef struct {
    CallFrame frames[FRAMES_MAX];
    int frameCount;
    Value stack[STACK_MAX];
    Value* stackTop;
    Table strings;
    Table globals;
    Upvalue* openUpvalues;
    int arrayIndex;
    Settings settings;
    GarbageCollector* garbageCollector;
} VM;

#endif
