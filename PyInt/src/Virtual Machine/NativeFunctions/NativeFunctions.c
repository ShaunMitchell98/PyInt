#include <string.h>
#include <time.h>

#include "NativeFunctions.h"
#include "../VM/VM.h"
#include "../../Types/Value/Value.h"
#include "../Settings/Settings.h"
#include "../Printing/VmPrinting.h"
#include "../../Services/Table/Table.h"
#include "../../Types/String/String.h"
#include "../../Types/NativeFunction/NativeFunction.h"

static void DefineNative(VM* vm, const char* name, NativeFn function) {
    Push(vm, OBJ_VAL(CopyStringToTable(vm->heap, &vm->strings, name, (int)strlen(name))));
    Push(vm, OBJ_VAL(NewNativeFunction(vm->heap, function, name)));
    SetTableEntry(&vm->globals, AS_STRING(vm->stack[0]), vm->stack[1]);
    Pop(vm);
    Pop(vm);
}

static Value Clock(IOSettings* settings, int argCount, Value* args) {
    return NUMBER_VAL((double)clock() / CLOCKS_PER_SEC);
}

static Value Print(IOSettings* settings, int argCount, Value* args) {
    if (settings->location == LOCATION_TERMINAL) {
        bool success = PrintValueToTerminal(settings, *args);
    }
    else if (settings->location == LOCATION_FILE) {
        bool success = PrintValueToFile(settings, *args);
    }
    else if (settings->location == LOCATION_STRING) {
        bool success = PrintValueToString(settings, *args);
    }
    return NONE_VAL;
}

void DefineStandardFunctions(VM* vm) {
    DefineNative(vm, "clock", Clock);
    DefineNative(vm, "print", Print);
}