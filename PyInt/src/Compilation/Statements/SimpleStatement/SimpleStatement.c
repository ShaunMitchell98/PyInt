#include "SimpleStatement.h"
#include "../../Helpers/Helpers.h"
#include "../GlobalStatement/GlobalStatement.h"
#include "../ReturnStatement/ReturnStatement.h"
#include "../ExpressionStatement/ExpressionStatement.h"

static void SmallStatement(Compiler* compiler, Services* services, Bytecode* bytecode) {
    if (MatchToken(services, services->parser->current, GLOBAL_TOKEN)) {
        GlobalStatement(services, bytecode);
    }
    else if (MatchToken(services, services->parser->current, RETURN_TOKEN)) {
        ReturnStatement(compiler, services, bytecode);
    }
    else if (MatchToken(services, services->parser->current, PASS_TOKEN)) {
        GetNextToken(services);
    }
    else {
        ExpressionStatement(compiler, services, bytecode);
    }
}

void SimpleStatement(Compiler* compiler, Services* services, Bytecode* bytecode) {
    do {
        SmallStatement(compiler, services, bytecode);
    } while (MatchToken(services, services->parser->current, SEMICOLON_TOKEN));
}