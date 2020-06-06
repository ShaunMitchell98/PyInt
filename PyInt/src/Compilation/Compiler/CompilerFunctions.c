#include <stdlib.h>
#include <string.h>

#include "Compiler.h"
#include "../../Services/Disassembly/BytecodeDisassembly/BytecodeDisassembly.h"
#include "../../Services/Table/TableFunctions.h"
#include "../Expressions/Expressions.h"
#include "../Bytecode/CompilerBytecode.h"
#include "../Helpers/Helpers.h"
#include "../Statements/Statement/Statement.h"
#include "../../Types/Function/FunctionFunctions.h"
#include "../../Types/Class/ClassFunctions.h"

static void InitScriptCompiler(Compiler* compiler, Services* services) {
    compiler->function = NewFunction(services->garbageCollector, SCRIPT_FUNCTION);
    compiler->localCount = 0;
    compiler->enclosing = NULL;
    compiler->type = SCRIPT_COMPILER;
    compiler->function->name = CopyStringToTable(services->garbageCollector, services->stringsTable, "Script", 6);

    compiler->locals->isCaptured = false;
    compiler->locals->name.start = "";
    compiler->locals->name.length = 0;
    compiler->localCount++;

    services->garbageCollector->compiler = compiler;
}

void InitFunctionCompiler(Compiler* currentCompiler, Compiler* compiler, Services* services) {
    Token previous = services->parser->previous;
    FunctionType type;

    if (previous.length == 8 && memcmp(previous.start, "__init__", 8) == 0) {
        type = INITIALISER_FUNCTION;
    }
    else {
        type = STANDARD_FUNCTION;
    }

    compiler->function = NewFunction(services->garbageCollector, type);
    compiler->localCount = 0;
    compiler->enclosing = (struct Compiler*) currentCompiler;
    compiler->function->name = CopyStringToTable(services->garbageCollector, services->stringsTable, services->parser->previous.start, services->parser->previous.length);
    
    compiler->locals->isCaptured = false;
    compiler->locals->name.start = "";
    compiler->locals->name.length = 0;
    compiler->localCount++;
    compiler->type = FUNCTION_COMPILER;

    services->garbageCollector->compiler = compiler;
}

Function* EndCompiler(Compiler* compiler, Services* services) {
    if (!compiler->function->hasReturnStatement) {
        WriteReturn(compiler->function->type, &compiler->function->bytecode, services);
    }

    Function* function = compiler->function;

    if (services->bytecodeSettings->enabled) {
        if (!services->parser->hadError) {
            DisassembleBytecode(compiler, compiler->locals, compiler->upvalues, &compiler->function->bytecode, compiler->function->name->chars, services->bytecodeSettings);
        }
    }

    if (compiler->enclosing != NULL) {       
        compiler = (Compiler*) compiler->enclosing;
        services->garbageCollector->compiler = compiler;
    }
  
    return function;
}

Function* Compile(RunMode runMode, Services* services) {
    Compiler compiler;
    InitScriptCompiler(&compiler, services);

    GetNextToken(services);
    if (runMode == RUN_REPL) {
        Statement(&compiler, services, &compiler.function->bytecode);
    }
    else {
        while (services->parser->current.type != EOF_TOKEN) {
            if (!MatchToken(services, services->parser->current, NEWLINE_TOKEN) && !MatchToken(services, services->parser->current, EOF_TOKEN)) {
                Statement(&compiler, services, &compiler.function->bytecode);
            }
        }
    }
    
    Function* function = EndCompiler(&compiler, services);
    return services->parser->hadError ? NULL : function;
}