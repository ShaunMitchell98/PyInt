#ifndef PyInt_CompilerFunctions_h
#define PyInt_CompilerFunction_h

#include "Compiler.h"
#include "../../Headers/RunMode.h"
#include "../Services/Services.h"

Function* Compile(RunMode runMode, Services* services);
void InitFunctionCompiler(Compiler* currentCompiler, Compiler* compiler, Services* services);
Function* EndCompiler(Compiler* compiler, Services* services);

#endif