#include "VM.h"
#include "../../Services/Scanner/Scanner.h"
#include "../../Compilation/Compiler/CompilerFunctions.h"
#include "../../Services/Memory/Memory.h"
#include "../../Compilation/Services/Services.h"
#include "../Services/Errors/RuntimeError.h"
#include "../Services/Stack/Stack.h"
#include "../NativeFunctions/NativeFunctions.h"
#include "../Services/Call/Call.h"
#include "../Run/Run.h"

void InitVM(VM* vm, Settings* settings) {
    ResetStack(vm);
    vm->arrayIndex = 0;
    vm->settings = *settings;
    vm->heap = NULL;
    vm->frameCount = 0;
    vm->openUpvalues = NULL;
    InitTable(&vm->strings);
    InitTable(&vm->globals);
    DefineStandardFunctions(vm);
}

void FreeVM(VM* vm) {
    ResetStack(vm);
    FreeObjects(vm->heap);
    FreeTable(&vm->strings);
    FreeTable(&vm->globals);
}

InterpretResult Interpret(const char* sourceCode, Settings* settings) {
    VM vm;
    InitVM(&vm, settings);

    Scanner scanner;
    InitScanner(&scanner, sourceCode);

    Parser parser;
    InitParser(&parser);

    Services services;
    InitServices(&services, &scanner, &parser, &settings->bytecode, &settings->output, vm.heap, &vm.strings);

    Function* function = Compile(settings->runMode, &services);
    
    if (function == NULL) {
        return INTERPRET_COMPILE_ERROR;
    }
    
    Push(&vm, OBJ_VAL(function));
    Closure* closure = NewClosure(vm.heap, function);
    Pop(&vm);
    Push(&vm, OBJ_VAL(closure));
    CallValue(&vm, OBJ_VAL(closure), 0);
    
    bool RuntimeError = Run(&vm);
    FreeVM(&vm);
    if (RuntimeError) {
        return INTERPRET_RUNTIME_ERROR;
    }
    else {
        return INTERPRET_OK;
    }
}
