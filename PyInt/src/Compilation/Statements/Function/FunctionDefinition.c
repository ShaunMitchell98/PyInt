#include <string.h>
#include "FunctionDefinition.h"
#include "../../Errors/Errors.h"
#include "../../../Types/Function/Function.h"
#include "../../Variables/Local/LocalFunctions.h"
#include "../../Bytecode/CompilerBytecode.h"
#include "../../Helpers/Helpers.h"
#include "../../Expressions/Expressions.h"
#include "../../Variables/Global/Global.h"
#include "../../Compiler/CompilerFunctions.h"
#include "../../../Services/Memory/Memory.h"
#include "../../Compiler/ClassCompiler/ClassCompiler.h"
#include "../../../Services/Table/TableFunctions.h"
#include "../../../Types/Closure/ClosureFunctions.h"

void EndScope(Local* locals, int localCount, Bytecode* bytecode, Services* services) {
    while (localCount > 0) {
        if ((locals + localCount - 1)->isCaptured) {
            WriteByte(bytecode, services, CLOSE_UPVALUE_OP);
        }
        else {
            WriteByte(bytecode, services, POP_OP);
        }
        localCount--;
    }
}

static void InitialiseSelf(Compiler* compiler, Services* services) {
    char* heapChars = ALLOCATE(services->garbageCollector, char, services->parser->current.length + 1);
    memcpy(heapChars, services->parser->current.start, services->parser->current.length);
    heapChars[services->parser->current.length] = '\0';

    compiler->locals->name.start = heapChars;
    compiler->locals->name.length = services->parser->current.length;
    compiler->function->arity--;
}

static void CompileFunction(Compiler* compiler, Services* services) {
    Compiler newCompiler;
    Bytecode* bytecode = &compiler->function->bytecode;
    InitFunctionCompiler(compiler, &newCompiler, services);

    ConsumeToken(services, LEFT_PAREN_TOKEN, LeftParenError);
    if (!CheckToken(services, RIGHT_PAREN_TOKEN)) {
        do {
            newCompiler.function->arity++;
            if (newCompiler.function->arity > 255) {
                Error("Cannot have more than 255 parameters");
            }

            if (newCompiler.function->arity == 1 && compiler->type == CLASS_COMPILER) {
                InitialiseSelf(&newCompiler, services);
            }
            else {
                PushLocalToCompilerStack(&newCompiler, &services->parser->current);
            }

            GetNextToken(services);
        } while (MatchToken(services, services->parser->current, COMMA_TOKEN));
    }
    ConsumeToken(services, RIGHT_PAREN_TOKEN, RightParenError);

    ConsumeToken(services, COLON_TOKEN, ColonError);
    Suite(&newCompiler, services, &newCompiler.function->bytecode);

    Function* function = EndCompiler(&newCompiler, services);

    if (compiler->type == CLASS_COMPILER) {
        ClassCompiler* classCompiler = (ClassCompiler*)compiler;
        Closure* closure = NewClosure(services->garbageCollector, function);
        SetTableEntry(services->garbageCollector, &classCompiler->klass->methods, function->name, OBJ_VAL(closure));
    }

    WriteBytes(bytecode, services, CLOSURE_OP, StoreInBytecodeConstantsTable(bytecode, services, OBJ_VAL(function)));

    for (int i = 0; i < function->upvalueCount; i++) {
        WriteByte(bytecode, services, compiler->upvalues[i].isLocal ? 1 : 0);
        WriteByte(bytecode, services, compiler->upvalues[i].index);
    }

    EndScope(newCompiler.locals, newCompiler.localCount, &newCompiler.function->bytecode, services);
}

void FunctionDefinition(Compiler* compiler, Services* services, Bytecode* bytecode) {
    int address;
    if (compiler->enclosing != NULL) {
        address = PushLocalToCompilerStack(compiler, &services->parser->current);
    }
    else {
        address = StoreGlobalInBytecodeConstantTable(bytecode, services, &services->parser->current);
    }

    GetNextToken(services);
    CompileFunction(compiler, services);

    if (compiler->enclosing != NULL) {
        WriteBytes(bytecode, services, SET_LOCAL_OP, address);
    }
    else {
        WriteBytes(bytecode, services, SET_GLOBAL_OP, address);
    }
}
