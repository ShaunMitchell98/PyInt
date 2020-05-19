#include "ReturnStatement.h"
#include "../../Expressions/Expressions.h"
#include "../../Bytecode/CompilerBytecode.h"
#include "../../Errors/Errors.h"
#include "../../Compiler/Compiler.h"

void ReturnStatement(Compiler* compiler, Services* services, Bytecode* bytecode) {
    if (compiler->enclosing == NULL) {
        Error("Cannot return from top-level code.");
    }

    if (compiler->function->type == INITIALISER_FUNCTION) {
        Error("Cannot return a value from an initialiser");
    }

    TestListStarExpression(compiler, services, bytecode);
    WriteByte(bytecode, services, RETURN_OP);
    compiler->function->hasReturnStatement = true;
}