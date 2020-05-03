#ifndef PyInt_Helpers_h
#define PyInt_Helpers_h

#include "../Services/Services.h"
#include "../../Headers/Token.h"

void GetNextToken(Services* compiler);
bool MatchToken(Services* compiler, Token token, TokenType tokenType);
void ConsumeToken(Services* compiler, TokenType tokenType, const char* message);
void TryConsumeToken(Services* compiler, TokenType tokenType);
bool CheckToken(Services* compiler, TokenType tokenType);
#endif
