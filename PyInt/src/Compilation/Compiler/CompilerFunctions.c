#include <stdlib.h>
#include <string.h>

#include "Compiler.h"
#include "../../Services/Disassembly/BytecodeDisassembly/BytecodeDisassembly.h"
#include "../../Services/Table/Table.h"
#include "../Expressions/Expressions.h"
#include "../Bytecode/CompilerBytecode.h"
#include "../Helpers/Helpers.h"
#include "../Statements/Statement/Statement.h"

static void InitScriptCompiler(Compiler* compiler, Services* services) {
    compiler->function = NewFunction(services->heap);
    compiler->localCount = 0;
    compiler->enclosing = NULL;
    compiler->function->name = CopyStringToTable(services->heap, services->stringsTable, "Script", 6);
    compiler->services = services;

    compiler->locals->isCaptured = false;
    compiler->locals->name.start = "";
    compiler->locals->name.length = 0;
    compiler->localCount++;
}

void InitFunctionCompiler(Compiler* currentCompiler, Compiler* compiler) {
    compiler->function = NewFunction(currentCompiler->services->heap);
    compiler->localCount = 0;
    compiler->enclosing = (struct Compiler*) currentCompiler;
    compiler->services = currentCompiler->services;
    compiler->function->name = CopyStringToTable(compiler->services->heap, compiler->services->stringsTable, compiler->services->parser->previous.start, compiler->services->parser->previous.length);
    
    compiler->locals->isCaptured = false;
    compiler->locals->name.start = "";
    compiler->locals->name.length = 0;
    compiler->localCount++;
}

Function* EndCompiler(Compiler* compiler) {
    if (!compiler->function->hasReturnStatement) {
        WriteReturn(&compiler->function->bytecode, compiler->services);
    }

    Function* function = compiler->function;

    if (compiler->services->bytecodeSettings->enabled) {
        if (!compiler->services->parser->hadError) {
            DisassembleBytecode(compiler, compiler->locals, compiler->upvalues, &compiler->function->bytecode, compiler->function->name->chars, compiler->services->bytecodeSettings);
        }
    }

    if (compiler->enclosing != NULL) {       
        compiler = (Compiler*) compiler->enclosing;
    }
  
    return function;
}

Function* Compile(RunMode runMode, Services* services) {
    Compiler compiler;
    InitScriptCompiler(&compiler, services);

    GetNextToken(services);
    if (runMode == RUN_REPL) {
        TestList(&compiler, services, &compiler.function->bytecode);
    }
    else {
        while (services->parser->current.type != EOF_TOKEN) {
            if (!MatchToken(services, services->parser->current, NEWLINE_TOKEN) && !MatchToken(services, services->parser->current, EOF_TOKEN)) {
                Statement(&compiler, services, &compiler.function->bytecode);
            }
        }
    }
    
    Function* function = EndCompiler(&compiler);
    return services->parser->hadError ? NULL : function;
}
