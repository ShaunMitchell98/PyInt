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

void ClassStatement(Compiler* compiler, Services* services, Bytecode* bytecode) {
	ConsumeToken(services, IDENTIFIER_TOKEN, ClassIdentifierError);
    int address;
    if (compiler->enclosing != NULL) {
        address = PushLocalToCompilerStack(compiler, &services->parser->previous);
        compiler->localCount++;
    }
    else {
        address = StoreGlobalInBytecodeConstantTable(bytecode, services, &services->parser->previous);
    }

    ClassCompiler newCompiler;
    InitClassCompiler(compiler, &newCompiler, services);
    ConsumeToken(services, COLON_TOKEN, ColonError);
    Suite(&newCompiler, services, &newCompiler.compiler.function->bytecode);
    Class* klass = EndClassCompiler(&newCompiler, services);
    WriteBytes(bytecode, services, CONSTANT_OP, StoreInBytecodeConstantsTable(bytecode, services, OBJ_VAL(klass)));
    
    if (compiler->enclosing != NULL) {
        WriteBytes(bytecode, services, SET_LOCAL_OP, address);
    }
    else {
        WriteBytes(bytecode, services, SET_GLOBAL_OP, address);
    }

}