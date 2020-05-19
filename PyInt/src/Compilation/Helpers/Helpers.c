#include "Helpers.h"
#include "../../Headers/Common.h"
#include "../Errors/Errors.h"
#include "../../Services/Scanner/ScannerFunctions.h"

void GetNextToken(Services* services) {
    
    if (services->parser->current.type == EOF_TOKEN) {
        return;
    }
    
    services->parser->previous = services->parser->current;
    
    services->parser->current = GetToken(services->scanner);
    
    if (services->parser->current.type == ERROR_TOKEN) {
        SyntaxError(services->outputSettings, &services->parser->current);
        services->parser->hadError = true;
    }
}

bool MatchToken(Services* services, Token token, TokenType tokenType) {
    if (token.type != tokenType) return false;
    GetNextToken(services);
    return true;
}

void ConsumeToken(Services* services, TokenType tokenType, const char* message) {
    if (services->parser->current.type == tokenType) {
        GetNextToken(services);
        return;
    }
    Error(message);
    services->parser->hadError = true;
}

void TryConsumeToken(Services* services, TokenType tokenType) {
    if (services->parser->current.type == tokenType) {
        GetNextToken(services);
        return;
    }
}

bool CheckToken(Services* services, TokenType tokenType) {
    if (services->parser->current.type == tokenType) return true;
    return false;
}
