#include "Helpers.h"
#include "Token.h"
#include "Compiler.h"
#include "CompilerErrors.h"

void GetNextToken() {
    
    if (parser.current.type == EOF_TOKEN) {
        return;
    }
    
    parser.previous = parser.current;
    
    parser.current = GetToken();
    
    if (parser.current.type == ERROR_TOKEN) {
        SyntaxError(&parser.current);
        parser.hadError = true;
    }
}
bool MatchToken(TokenType tokenType) {
    if (parser.current.type != tokenType) return false;
    GetNextToken();
    return true;
}

void ConsumeToken(TokenType tokenType, const char* message) {
    if (parser.current.type == tokenType) {
        GetNextToken();
        return;
    }
    Error(message);
    parser.hadError = true;
}

void TryConsumeToken(TokenType tokenType) {
    if (parser.current.type == tokenType) {
        GetNextToken();
        return;
    }
}

bool CheckToken(TokenType tokenType) {
    if (parser.current.type == tokenType) return true;
    return false;
}
