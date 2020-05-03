#ifndef PyInt_Expressions_h
#define PyInt_Expressions_h

#include "../Services/Services.h"
#include "../../Services/Bytecode/Bytecode.h"
#include "../Compiler/Compiler.h"

void Test(Compiler* compiler, Services* services, Bytecode* bytecode);
void TestList(Compiler* compiler, Services* services, Bytecode* bytecode);
void TestListStarExpression(Compiler* compiler, Services* services, Bytecode* bytecode);
void NamedExpressionTest(Compiler* compiler, Services* services, Bytecode* bytecode);
void Suite(Compiler* compiler, Services* services, Bytecode* bytecode);

#endif