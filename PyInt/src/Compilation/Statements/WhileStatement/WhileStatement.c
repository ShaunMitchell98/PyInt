#include "WhileStatement.h"
#include "../../Errors/Errors.h"
#include "../../Bytecode/CompilerBytecode.h"
#include "../../Expressions/Expressions.h"
#include "../../Helpers/Helpers.h"

void WhileStatement(Compiler* compiler, Services* services, Bytecode* bytecode) {
    int loopStart = bytecode->count;

    NamedExpressionTest(compiler, services, bytecode);
    ConsumeToken(services, COLON_TOKEN, ColonError);

    int ifFalseOpcodeAddress = WriteJump(bytecode, services, JUMP_IF_FALSE_OP);

    Suite(compiler, services, bytecode);
    WriteLoop(bytecode, services, loopStart);
    PatchJump(bytecode, ifFalseOpcodeAddress);
}
