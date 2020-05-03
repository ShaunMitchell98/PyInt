#include <stdlib.h>

#include "IfStatement.h"
#include "../../Expressions/Expressions.h"
#include "../../Errors/Errors.h"
#include "../../Helpers/Helpers.h"
#include "../../Bytecode/CompilerBytecode.h"

static int* AddOpcodeAddress(int* addresses, int address, int counter) {
    *(addresses + counter) = address;
    return (int*)realloc(addresses, sizeof(int) * (counter + 2));
}

void IfStatement(Compiler* compiler, Services* services, Bytecode* bytecode) {
    NamedExpressionTest(compiler, services, bytecode);
    ConsumeToken(services, COLON_TOKEN, ColonError);

    int ifFalseOpcodeAddress = WriteJump(bytecode, services, JUMP_IF_FALSE_OP);
    int elifFalseOpcodeAddress = 0;

    Suite(compiler, services, bytecode);

    int* trueOpcodeAddresses = (int*)malloc(sizeof(int));
    int counter = 0;

    trueOpcodeAddresses = AddOpcodeAddress(trueOpcodeAddresses, WriteJump(bytecode, services, JUMP_OP), counter);
    counter++;

    PatchJump(bytecode, ifFalseOpcodeAddress);

    while (MatchToken(services, services->parser->current, ELIF_TOKEN)) {
        NamedExpressionTest(compiler, services, bytecode);
        ConsumeToken(services, COLON_TOKEN, ColonError);
        elifFalseOpcodeAddress = WriteJump(bytecode, services, JUMP_IF_FALSE_OP);
        Suite(compiler, services, bytecode);

        trueOpcodeAddresses = AddOpcodeAddress(trueOpcodeAddresses, WriteJump(bytecode, services, JUMP_OP), counter);
        counter++;
        PatchJump(bytecode, elifFalseOpcodeAddress);
    }

    if (MatchToken(services, services->parser->current, ELSE_TOKEN)) {
        ConsumeToken(services, COLON_TOKEN, ColonError);
        Suite(compiler, services, bytecode);
    }

    for (int i = 0; i < counter; i++) {
        PatchJump(bytecode, *(trueOpcodeAddresses + i));
    }

    free(trueOpcodeAddresses);
}

