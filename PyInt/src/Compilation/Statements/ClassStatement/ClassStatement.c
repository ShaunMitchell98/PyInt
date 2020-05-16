#include "ClassStatement.h"
#include "../../Helpers/Helpers.h"
#include "../../Errors/Errors.h"
#include "../../Variables/Local/LocalFunctions.h"
#include "../../Variables/Global/Global.h"
#include "../../Bytecode/CompilerBytecode.h"

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

    WriteBytes(bytecode, services, CLASS_OP, address);
    ConsumeToken(services, LEFT_PAREN_TOKEN, LeftParenError);
    ConsumeToken(services, RIGHT_PAREN_TOKEN, RightParenError);
    ConsumeToken(services, COLON_TOKEN, ColonError);
    ConsumeToken(services, NEWLINE_TOKEN, NewlineError);
    ConsumeToken(services, INDENT_TOKEN, IndentError);
    ConsumeToken(services, PASS_TOKEN, "");
    ConsumeToken(services, NEWLINE_TOKEN, NewlineError);
    ConsumeToken(services, DEDENT_TOKEN, "");
}