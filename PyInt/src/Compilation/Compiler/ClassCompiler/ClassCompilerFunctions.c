#include "ClassCompilerFunctions.h"
#include "../CompilerFunctions.h"
#include "../../../Services/Table/TableFunctions.h"
#include "../../../Types/Class/ClassFunctions.h"
#include "../../../Types/Closure/ClosureFunctions.h"
#include "../../Bytecode/CompilerBytecode.h"

void InitClassCompiler(Compiler* currentCompiler, ClassCompiler* classCompiler, Services* services) {
    InitFunctionCompiler(currentCompiler, (Compiler*)classCompiler, services);
    classCompiler->name = services->parser->previous;
    classCompiler->compiler.type = CLASS;
    String* className = CopyStringToTable(services->garbageCollector, services->stringsTable, services->parser->previous.start, services->parser->previous.length);
    Class* klass = NewClass(services->garbageCollector, className);
    classCompiler->klass = klass;
}

Class* EndClassCompiler(ClassCompiler* currentCompiler, Services* services) {
    WriteByte(&currentCompiler->compiler.function->bytecode, services, RETURN_OP);
    currentCompiler->compiler.function->hasReturnStatement = true;
    Function* function = EndCompiler((Compiler*)currentCompiler, services);
    Closure* closure = NewClosure(services->garbageCollector, function);
    currentCompiler->klass->init = closure;
    return currentCompiler->klass;
}