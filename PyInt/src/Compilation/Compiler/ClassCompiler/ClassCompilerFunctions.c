#include <stdlib.h>

#include "ClassCompilerFunctions.h"
#include "../CompilerFunctions.h"
#include "../../../Services/Table/TableFunctions.h"
#include "../../../Types/Class/ClassFunctions.h"
#include "../../../Types/Closure/ClosureFunctions.h"
#include "../../Bytecode/CompilerBytecode.h"

void InitClassCompiler(Compiler* currentCompiler, ClassCompiler* classCompiler, Token className, Services* services) {
    InitFunctionCompiler(currentCompiler, (Compiler*)classCompiler, services);
    classCompiler->name = className;
    classCompiler->compiler.type = CLASS;
    classCompiler->superclassName = NULL;
    String* name = CopyStringToTable(services->garbageCollector, services->stringsTable, className.start, className.length);
    Class* klass = NewClass(services->garbageCollector, name);
    classCompiler->klass = klass;
}

Class* EndClassCompiler(ClassCompiler* currentCompiler, Services* services) {
    WriteByte(&currentCompiler->compiler.function->bytecode, services, RETURN_OP);
    currentCompiler->compiler.function->hasReturnStatement = true;
    Function* function = EndCompiler((Compiler*)currentCompiler, services);
    Closure* closure = NewClosure(services->garbageCollector, function);
    currentCompiler->klass->init[currentCompiler->klass->initCount++] = closure;
    return currentCompiler->klass;
}