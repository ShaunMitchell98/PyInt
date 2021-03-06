#include "VMFunctions.h"
#include "../../Services/Scanner/ScannerFunctions.h"
#include "../../Compilation/Compiler/CompilerFunctions.h"
#include "../../Services/Memory/Memory.h"
#include "../../Compilation/Services/Services.h"
#include "../Services/Errors/RuntimeError.h"
#include "../Services/Stack/Stack.h"
#include "../NativeFunctions/NativeFunctions.h"
#include "../Services/Call/Call.h"
#include "../../Services/GarbageCollection/GarbageCollection.h"
#include "../../Services/Table/TableFunctions.h"
#include "../../Types/Closure/ClosureFunctions.h"

static void InitVM(VM* vm, Settings* settings) {
    ResetStack(vm);
    vm->arrayIndex = 0;
    vm->settings = *settings;
    vm->frameCount = 0;
    vm->openUpvalues = NULL;
    InitTable(&vm->strings);
    InitTable(&vm->globals);

    GarbageCollector* garbageCollector = (GarbageCollector*)malloc(sizeof(GarbageCollector));
    InitGarbageCollector(garbageCollector, &settings->garbage);
    garbageCollector->globalsTable = &vm->globals;
    garbageCollector->stringsTable = &vm->strings;
    garbageCollector->stack = vm->stack;
    garbageCollector->stackTop = vm->stackTop;

    garbageCollector->initString = NULL;
    garbageCollector->initString = CopyStringToTable(garbageCollector, &vm->strings, "__init__", 8);
    vm->garbageCollector = garbageCollector;
    DefineStandardFunctions(vm);
}

static void FreeVM(VM* vm) {
    ResetStack(vm);
    FreeObjects(vm->garbageCollector);
    FreeTable(vm->garbageCollector, &vm->strings);
    FreeTable(vm->garbageCollector, &vm->globals);
    free(vm->garbageCollector);
}

InterpretResult Interpret(const char* sourceCode, Settings* settings) {
    Scanner scanner;
    VM vm;
    InitVM(&vm, settings);
    InitScanner(&scanner, sourceCode);

    Parser parser;
    InitParser(&parser);

    Services services;
    InitServices(&services, &scanner, &parser, &settings->bytecode, &settings->output, vm.garbageCollector, &vm.strings);

    Function* function = Compile(settings->runMode, &services);

    if (function == NULL) {
        return INTERPRET_COMPILE_ERROR;
    }

    Push(&vm, OBJ_VAL(function));
    Closure* closure = NewClosure(vm.garbageCollector, function);
    Pop(&vm);
    Push(&vm, OBJ_VAL(closure));
    bool RuntimeError = CallValue(&vm, OBJ_VAL(closure), 0);
    FreeVM(&vm);

    if (RuntimeError) {
        return INTERPRET_RUNTIME_ERROR;
    }
    else {
        return INTERPRET_OK;
    }
}
