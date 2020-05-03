#include "Expressions.h"
#include "../ParseFunctions/ParseFunctions.h"
#include "../Helpers/Helpers.h"
#include "../Statements/SimpleStatement/SimpleStatement.h"
#include "../Statements/Statement/Statement.h"
#include "../Errors/Errors.h"

static void StarExpression(Compiler* compiler, Services* services, Bytecode* bytecode) {
    ParsePrecedence(compiler, services, bytecode, PREC_ASSIGNMENT);
}

static void LambdaDef() {

}

void Test(Compiler* compiler, Services* services, Bytecode* bytecode) {
    if (MatchToken(services, services->parser->current, LAMBDA_TOKEN)) {
        LambdaDef();
    }
    else {
        ParsePrecedence(compiler, services, bytecode, PREC_ASSIGNMENT);
    }
    if (MatchToken(services, services->parser->current, IF_TOKEN)) {
        ParsePrecedence(compiler, services, bytecode, PREC_OR);
        if (MatchToken(services, services->parser->current, IF_TOKEN)) {
            Test(compiler, services, bytecode);
        }
    }
}

void Suite(Compiler* compiler, Services* services, Bytecode* bytecode) {

    if (MatchToken(services, services->parser->current, NEWLINE_TOKEN)) {
        ConsumeToken(services, INDENT_TOKEN, IndentError);
        do {
            Statement(compiler, services, bytecode);
            TryConsumeToken(services, NEWLINE_TOKEN);
        } while (!MatchToken(services, services->parser->current, DEDENT_TOKEN) && !MatchToken(services, services->parser->current, EOF_TOKEN));
    }
    else {
        SimpleStatement(compiler, services, bytecode);
    }
}

static void ExpressionList(Compiler* compiler, Services* services, Bytecode* bytecode) {
    do {
        if (MatchToken(services, services->parser->current, STAR_TOKEN)) {
            StarExpression(compiler, services, bytecode);
        }
        else {
            ParsePrecedence(compiler, services, bytecode, PREC_ASSIGNMENT);
        }
    } while (MatchToken(services, services->parser->current, COMMA_TOKEN));
}

void NamedExpressionTest(Compiler* compiler, Services* services, Bytecode* bytecode) {
    Test(compiler, services, bytecode);
    if (MatchToken(services, services->parser->current, COLON_EQUAL_TOKEN)) {
        Test(compiler, services, bytecode);
    }
}

void TestListStarExpression(Compiler* compiler, Services* services, Bytecode* bytecode) {
    do {
        if (MatchToken(services, services->parser->current, STAR_TOKEN)) {
            StarExpression(compiler, services, bytecode);
        }
        else {
            Test(compiler, services, bytecode);
        }
    } while (MatchToken(services, services->parser->current, COMMA_TOKEN));
}

void TestList(Compiler* compiler, Services* services, Bytecode* bytecode) {
    do {
        Test(compiler, services, bytecode);
    } while (MatchToken(services, services->parser->current, COMMA_TOKEN));
}