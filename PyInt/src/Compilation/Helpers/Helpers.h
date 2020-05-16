#ifndef PyInt_Helpers_h
#define PyInt_Helpers_h

#include "../Services/Services.h"
#include "../../Headers/Token.h"

void GetNextToken(Services* services);
bool MatchToken(Services* services, Token token, TokenType tokenType);
void ConsumeToken(Services* services, TokenType tokenType, const char* message);
void TryConsumeToken(Services* services, TokenType tokenType);
bool CheckToken(Services* services, TokenType tokenType);
#endif
