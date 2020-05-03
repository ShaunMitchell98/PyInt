#ifndef PyInt_CompilerFunctions_h
#define PyInt_CompilerFunction_h

#include "Compiler.h"
#include "../../Headers/RunMode.h"

Function* Compile(RunMode runMode, Services* services);
void InitFunctionCompiler(Compiler* currentCompiler, Compiler* compiler);
Function* EndCompiler(Compiler* compiler);

#endif