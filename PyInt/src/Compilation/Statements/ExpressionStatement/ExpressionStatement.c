#include "ExpressionStatement.h"
#include "../../ParseFunctions/ParseFunctions.h"
#include "../../Expressions/Expressions.h"
#include "../../Helpers/Helpers.h"

void ExpressionStatement(Compiler* compiler, Services* services, Bytecode* bytecode) {
    TestListStarExpression(compiler, services, bytecode);

    if (MatchToken(services, services->parser->current, COLON_TOKEN)) {
        Test(compiler, services, bytecode);
    }
    if (MatchToken(services, services->parser->current, EQUAL_TOKEN)) {
        ParsePrecedence(compiler, services, bytecode, PREC_ASSIGNMENT);
    }
}