#include <string.h>
#include <time.h>

#include "../Headers/HashTable.h"
#include "../Headers/Object.h"
#include "../Headers/NativeFunctions.h"
#include "../Headers/VM.h"
#include "../Headers/VmPrint.h"

static void DefineNative(VM* vm, const char* name, NativeFn function) {
    Push(vm, OBJ_VAL(CopyStringToTable(vm, &vm->strings, name, (int)strlen(name))));
    Push(vm, OBJ_VAL(NewNativeFunction(vm, function, name)));
    SetTableEntry(&vm->globals, AS_STRING(vm->stack[0]), vm->stack[1]);
    Pop(vm);
    Pop(vm);
}

static Value Clock(VM* vm, int argCount, Value* args) {
    return NUMBER_VAL((double)clock() / CLOCKS_PER_SEC);
}

static Value Print(VM* vm, int argCount, Value* args) {
    if (vm->settings.output.location == LOCATION_TERMINAL) {
        bool success = PrintValueToTerminal(vm, *args);
    }
    else if (vm->settings.output.location == LOCATION_FILE) {
        bool success = PrintValueToFile(vm, *args);
    }
    else if (vm->settings.output.location == LOCATION_STRING) {
        bool success = PrintValueToString(vm, *args);
    }
    return NONE_VAL;
}

void DefineStandardFunctions(VM* vm) {
    DefineNative(vm, "clock", Clock);
    DefineNative(vm, "print", Print);
}