#include "ClassCompilerFunctions.h"
#include "../CompilerFunctions.h"
#include "../../../Services/Table/TableFunctions.h"
#include "../../../Types/Class/ClassFunctions.h"

void InitClassCompiler(Compiler* currentCompiler, ClassCompiler* classCompiler, Services* services) {
    InitFunctionCompiler(currentCompiler, (Compiler*)classCompiler, services);
    classCompiler->name = services->parser->previous;
    classCompiler->compiler.type = CLASS;
    String* className = CopyStringToTable(services->garbageCollector, services->stringsTable, services->parser->previous.start, services->parser->previous.length);
    Class* klass = NewClass(services->garbageCollector, className);
    classCompiler->klass = klass;
}

Class* EndClassCompiler(ClassCompiler* currentCompiler, Services* services) {
    EndCompiler((Compiler*)currentCompiler, services);
    return currentCompiler->klass;
}