#include "Statement.h"
#include "../CompoundStatement/CompoundStatement.h"
#include "../SimpleStatement/SimpleStatement.h"

void Statement(Compiler* compiler, Services* services, Bytecode* bytecode) {
    if (services->parser->current.type == NEWLINE_TOKEN) return;
    bool valid = CompoundStatement(compiler, services, bytecode);
    if (!valid) {
        SimpleStatement(compiler, services, bytecode);
    }
}