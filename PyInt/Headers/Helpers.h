#ifndef PyInt_Helpers_h
#define PyInt_Helpers_h

#include "Token.h"
#include "Common.h"
#include "Scanner.h"

void GetNextToken(Scanner* scanner);
bool MatchToken(TokenType tokenType);
void ConsumeToken(TokenType tokenType, const char* message);
void TryConsumeToken(TokenType tokenType);
bool CheckToken(TokenType tokenType);
#endif
