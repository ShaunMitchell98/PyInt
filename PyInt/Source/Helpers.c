#include "../Headers/Helpers.h"
#include "../Headers/Token.h"
#include "../Headers/Compiler.h"
#include "../Headers/CompilerErrors.h"

void GetNextToken(Compiler* compiler) {
    
    if (compiler->parser->current.type == EOF_TOKEN) {
        return;
    }
    
    compiler->parser->previous = compiler->parser->current;
    
    compiler->parser->current = GetToken(compiler->scanner);
    
    if (compiler->parser->current.type == ERROR_TOKEN) {
        SyntaxError(&compiler->parser->current);
        compiler->parser->hadError = true;
    }
}
bool MatchToken(Compiler* compiler, Token token, TokenType tokenType) {
    if (token.type != tokenType) return false;
    GetNextToken(compiler);
    return true;
}

void ConsumeToken(Compiler* compiler, TokenType tokenType, const char* message) {
    if (compiler->parser->current.type == tokenType) {
        GetNextToken(compiler);
        return;
    }
    Error(message);
    compiler->parser->hadError = true;
}

void TryConsumeToken(Compiler* compiler, TokenType tokenType) {
    if (compiler->parser->current.type == tokenType) {
        GetNextToken(compiler);
        return;
    }
}

bool CheckToken(Compiler* compiler, TokenType tokenType) {
    if (compiler->parser->current.type == tokenType) return true;
    return false;
}
