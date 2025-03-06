#include <string.h>
#include <time.h>

#include "NativeFunctions.h"
#include "../VM/VM.h"
#include "../../Types/Value/Value.h"
#include "../Settings/Settings.h"
#include "../Printing/VmPrinting.h"
#include "../../Services/Table/TableFunctions.h"
#include "../../Types/String/String.h"
#include "../../Types/NativeFunction/NativeFunction.h"
#include "../../Types/NativeFunction/NativeFunctionFunctions.h"
#include "../Services/Stack/Stack.h"
#include "../Services/Errors/RuntimeError.h"

static void DefineNative(VM* vm, const char* name, NativeFn function) {
    Push(vm, OBJ_VAL(CopyStringToTable(vm->garbageCollector, &vm->strings, name, (int)strlen(name))));
    Push(vm, OBJ_VAL(NewNativeFunction(vm->garbageCollector, function, name)));
    SetTableEntry(vm->garbageCollector, &vm->globals, AS_STRING(vm->stack[0]), vm->stack[1]);
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

static Value Len(IOSettings* settings, int argCount, Value* args) {
    if (argCount != 1) {
        RuntimeError(settings, "len() takes exactly one argument (%d given).", argCount);
        return NONE_VAL;
    }
    
    if (!IS_OBJ(args[0])) {
        RuntimeError(settings, "Object of type '%s' has no len().", 
            IS_NUMBER(args[0]) ? "number" : 
            IS_BOOLEAN(args[0]) ? "boolean" : 
            IS_CHAR(args[0]) ? "char" : "none");
        return NONE_VAL;
    }
    
    if (OBJ_TYPE(args[0]) == STRING) {
        String* string = AS_STRING(args[0]);
        return NUMBER_VAL(string->length);
    }
    
    RuntimeError(settings, "Object of type '%s' has no len().", 
        OBJ_TYPE(args[0]) == FUNCTION ? "function" :
        OBJ_TYPE(args[0]) == CLOSURE ? "closure" :
        OBJ_TYPE(args[0]) == CLASS ? "class" :
        OBJ_TYPE(args[0]) == NATIVE ? "native function" :
        OBJ_TYPE(args[0]) == CLASS_INSTANCE ? "instance" :
        OBJ_TYPE(args[0]) == BOUND_METHOD ? "method" : "object");
    
    return NONE_VAL;
}

void DefineStandardFunctions(VM* vm) {
    DefineNative(vm, "clock", Clock);
    DefineNative(vm, "print", Print);
    DefineNative(vm, "len", Len);
}