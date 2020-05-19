#ifndef PyInt_ClassCompilerFunctions_h
#define PyInt_ClassCompilerFunctions_h

#include "ClassCompiler.h"
#include "../Compiler.h"
#include "../../Services/Services.h"

void InitClassCompiler(Compiler* currentCompiler, ClassCompiler* classCompiler, Services* services);
Class* EndClassCompiler(ClassCompiler* currentCompiler, Services* services);
#endif
