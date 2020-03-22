#include "../Headers/Helpers.h"
#include "../Headers/Token.h"
#include "../Headers/Compiler.h"
#include "../Headers/CompilerErrors.h"

void GetNextToken(Scanner* scanner) {
    
    if (parser.current.type == EOF_TOKEN) {
        return;
    }
    
    parser.previous = parser.current;
    
    parser.current = GetToken(scanner);
    
    if (parser.current.type == ERROR_TOKEN) {
        SyntaxError(&parser.current);
        parser.hadError = true;
    }
}
bool MatchToken(TokenType tokenType) {
    if (parser.current.type != tokenType) return false;
    GetNextToken(currentCompiler->scanner);
    return true;
}

void ConsumeToken(TokenType tokenType, const char* message) {
    if (parser.current.type == tokenType) {
        GetNextToken(currentCompiler->scanner);
        return;
    }
    Error(message);
    parser.hadError = true;
}

void TryConsumeToken(TokenType tokenType) {
    if (parser.current.type == tokenType) {
        GetNextToken(currentCompiler->scanner);
        return;
    }
}

bool CheckToken(TokenType tokenType) {
    if (parser.current.type == tokenType) return true;
    return false;
}
