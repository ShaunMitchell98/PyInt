#include "CompoundStatement.h"
#include "../../Helpers/Helpers.h"
#include "../IfStatement/IfStatement.h"
#include "../WhileStatement/WhileStatement.h"
#include "../ForStatement/ForStatement.h"
#include "../Function/FunctionDefinition.h"

bool CompoundStatement(Compiler* compiler, Services* services, Bytecode* bytecode) {
    if (MatchToken(services, services->parser->current, IF_TOKEN)) {
        IfStatement(compiler, services, bytecode);
    }
    else if (MatchToken(services, services->parser->current, WHILE_TOKEN)) {
        WhileStatement(compiler, services, bytecode);
    }
    else if (MatchToken(services, services->parser->current, FOR_TOKEN)) {
        ForStatement(compiler, services, bytecode);
    }
    else if (MatchToken(services, services->parser->current, DEF_TOKEN)) {
        FunctionDefinition(compiler, services, bytecode);
    }
    else {
        return false;
    }

    return true;
}