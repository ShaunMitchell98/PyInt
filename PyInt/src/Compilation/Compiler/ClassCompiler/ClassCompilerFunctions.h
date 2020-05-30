#ifndef PyInt_ClassCompilerFunctions_h
#define PyInt_ClassCompilerFunctions_h

#include "ClassCompiler.h"
#include "../Compiler.h"
#include "../../Services/Services.h"
#include "../../../Headers/Token.h"

void InitClassCompiler(Compiler* currentCompiler, ClassCompiler* classCompiler, Token className, Services* services);
Class* EndClassCompiler(ClassCompiler* currentCompiler, Services* services);
#endif
