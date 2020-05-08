#include "ForStatement.h"
#include "../../Helpers/Helpers.h"
#include "../../Errors/Errors.h"
#include "../../Bytecode/CompilerBytecode.h"
#include "../../Variables/Local/LocalFunctions.h"
#include "../../../Types/Function/Function.h"
#include "../../Expressions/Expressions.h"
#include "../../ParseFunctions/ParseFunctions.h"
#include "../../Compiler/Compiler.h"
#include "../../Variables/Global/Global.h"

static uint8_t GetIdentifierAddress(Compiler* compiler) {
    if (compiler->enclosing != NULL) {
        return (uint8_t)GetLocalStackOffset(compiler->locals, compiler->localCount, &compiler->services->parser->previous);
    }
    else {
        return StoreGlobalInBytecodeConstantTable(&compiler->function->bytecode, compiler->services, &compiler->services->parser->previous);
    }
}

void ForStatement(Compiler* compiler, Services* services, Bytecode* bytecode) {
    GetNextToken(services);
    Token loopVariable = services->parser->previous;
    if (MatchToken(services, services->parser->current, EQUAL_TOKEN)) {
        ParsePrecedence(compiler, services, bytecode, PREC_ASSIGNMENT);
        SetNewLocalVariable(compiler, bytecode, services, &loopVariable);
    }
    ConsumeToken(services, IN_TOKEN, InError);
    GetNextToken(services);
    uint8_t identifierAddress = GetIdentifierAddress(compiler);
    ConsumeToken(services, COLON_TOKEN, ColonError);

    int loopStart = bytecode->count;
    WriteBytes(bytecode, services, END_OF_ARRAY_OP, identifierAddress);

    int ifTrueOpcodeAddress = WriteJump(bytecode, services, JUMP_IF_TRUE_OP);
    WriteBytes(bytecode, services, GET_INDEX_OP, identifierAddress);
    if (compiler->enclosing != NULL) {
        WriteBytes(bytecode, services, SET_LOCAL_OP, (uint8_t)GetLocalStackOffset(compiler->locals, compiler->localCount, &loopVariable));
    }
    else {
        WriteBytes(bytecode, services, SET_GLOBAL_OP, StoreGlobalInBytecodeConstantTable(bytecode, services, &loopVariable));
    }
    Suite(compiler, services, bytecode);
    WriteLoop(bytecode, services, loopStart);
    PatchJump(bytecode, ifTrueOpcodeAddress);
}