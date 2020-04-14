#include <string.h>
#include <time.h>

#include "../Headers/HashTable.h"
#include "../Headers/Object.h"
#include "../Headers/NativeFunctions.h"
#include "../Headers/VM.h"

static void DefineNative(VM* vm, const char* name, NativeFn function) {
    Push(vm, OBJ_VAL(CopyStringToTable(vm, &vm->strings, name, (int)strlen(name))));
    Push(vm, OBJ_VAL(NewNativeFunction(vm, function)));
    SetTableEntry(&vm->globals, AS_STRING(vm->stack[0]), vm->stack[1]);
    Pop(vm);
    Pop(vm);
}

static Value Clock(int argCount, Value* args) {
    return NUMBER_VAL((double)clock() / CLOCKS_PER_SEC);
}

void DefineStandardFunctions(VM* vm) {
    DefineNative(vm, "Clock", Clock);
}