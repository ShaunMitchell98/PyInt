#include <string.h>

#include "ClassStatement.h"
#include "../../Helpers/Helpers.h"
#include "../../Errors/Errors.h"
#include "../../Variables/Local/LocalFunctions.h"
#include "../../Variables/Global/Global.h"
#include "../../Bytecode/CompilerBytecode.h"
#include "../../Expressions/Expressions.h"
#include "../../Compiler/CompilerFunctions.h"
#include "../Function/FunctionDefinition.h"
#include "../../Compiler/ClassCompiler/ClassCompilerFunctions.h"
#include "../../../Services/Table/TableFunctions.h"

void ClassStatement(Compiler* compiler, Services* services, Bytecode* bytecode) {
	ConsumeToken(services, IDENTIFIER_TOKEN, ClassIdentifierError);
    int classNameAddress;
    if (compiler->enclosing != NULL) {
        classNameAddress = PushLocalToCompilerStack(compiler, &services->parser->previous);
        compiler->localCount++;
    }
    else {
        classNameAddress = StoreGlobalInBytecodeConstantTable(bytecode, services, &services->parser->previous);
    }

    Token className = services->parser->previous;
    Token superclassName;
    bool hasSuperclass = false;
    if (MatchToken(services, services->parser->current, LEFT_PAREN_TOKEN)) {
        if (IdentifiersEqual(&className, &services->parser->current)) {
            Error("A class cannot inherit from itself.");
        }
        superclassName = services->parser->current;
        hasSuperclass = true;
        GetNextToken(services);
        ConsumeToken(services, RIGHT_PAREN_TOKEN, RightParenError);
    }

    ClassCompiler newCompiler;
    InitClassCompiler(compiler, &newCompiler, className, services);
    ConsumeToken(services, COLON_TOKEN, ColonError);
    WriteBytes(&newCompiler.compiler.function->bytecode, services, SET_LOCAL_OP, 0);

    if (hasSuperclass) {
        newCompiler.superclassName = CopyStringToTable(services->garbageCollector, services->stringsTable, superclassName.start, superclassName.length);
    }

    newCompiler.compiler.localCount++;
    Suite(&newCompiler, services, &newCompiler.compiler.function->bytecode);
    WriteBytes(&newCompiler.compiler.function->bytecode, services, GET_LOCAL_OP, 0);
    Class* klass = EndClassCompiler(&newCompiler, services);
    WriteBytes(bytecode, services, CONSTANT_OP, StoreInBytecodeConstantsTable(bytecode, services, OBJ_VAL(klass)));
    
    if (compiler->enclosing != NULL) {
        WriteBytes(bytecode, services, SET_LOCAL_OP, classNameAddress);
    }
    else {
        WriteBytes(bytecode, services, SET_GLOBAL_OP, classNameAddress);
    }

}