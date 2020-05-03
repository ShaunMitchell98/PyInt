#include "FunctionDefinition.h"
#include "../../Errors/Errors.h"
#include "../../../Types/Function/Function.h"
#include "../../Variables/Local/LocalFunctions.h"
#include "../../Bytecode/CompilerBytecode.h"
#include "../../Helpers/Helpers.h"
#include "../../Expressions/Expressions.h"
#include "../../Variables/Global/Global.h"
#include "../../Compiler/CompilerFunctions.h"

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

static void CompileFunction(Compiler* compiler, Services* services) {
    Compiler newCompiler;
    Bytecode* bytecode = &compiler->function->bytecode;
    InitFunctionCompiler(compiler, &newCompiler);

    ConsumeToken(services, LEFT_PAREN_TOKEN, LeftParenError);
    if (!CheckToken(services, RIGHT_PAREN_TOKEN)) {
        do {
            newCompiler.function->arity++;
            if (newCompiler.function->arity > 255) {
                Error("Cannot have more than 255 parameters");
            }

            PushLocalToCompilerStack(&newCompiler, &newCompiler.services->parser->current);
            GetNextToken(newCompiler.services);
        } while (MatchToken(newCompiler.services, newCompiler.services->parser->current, COMMA_TOKEN));
    }
    ConsumeToken(newCompiler.services, RIGHT_PAREN_TOKEN, RightParenError);

    ConsumeToken(newCompiler.services, COLON_TOKEN, ColonError);
    Suite(&newCompiler, newCompiler.services, &newCompiler.function->bytecode);

    Function* function = EndCompiler(&newCompiler);
    WriteBytes(bytecode, services, CLOSURE_OP, StoreInBytecodeValueArray(bytecode, OBJ_VAL(function)));

    for (int i = 0; i < function->upvalueCount; i++) {
        WriteByte(bytecode, services, compiler->upvalues[i].isLocal ? 1 : 0);
        WriteByte(bytecode, services, compiler->upvalues[i].index);
    }

    EndScope(newCompiler.locals, newCompiler.localCount, &newCompiler.function->bytecode, newCompiler.services);
}

void FunctionDefinition(Compiler* compiler, Services* services, Bytecode* bytecode) {
    int address;
    if (compiler->enclosing != NULL) {
        address = PushLocalToCompilerStack(compiler, &services->parser->current);
        compiler->localCount++;
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
