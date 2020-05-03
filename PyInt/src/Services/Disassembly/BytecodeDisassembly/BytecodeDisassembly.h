#ifndef PyInt_BytecodeDisassembly_h
#define PyInt_BytecodeDisassembly_h

#include "../../Bytecode/Bytecode.h"
#include "../../../Compilation/Variables/Local/Local.h"
#include "../../../Virtual Machine/VM/VM.h"
#include "../../../Compilation/Variables/Upvalue/CompilerUpvalue.h"
#include "../../../Compilation/Compiler/Compiler.h"

void DisassembleBytecode(Compiler* compiler, Local* stack, CompilerUpvalue* upvalues, Bytecode* bytecode, const char* functionName, IOSettings* bytecodeSettings);
#endif