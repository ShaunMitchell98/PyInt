#ifndef PyInt_Helpers_h
#define PyInt_Helpers_h

#include "Token.h"
#include "Common.h"
#include "Compiler.h"
#include "Scanner.h"

void GetNextToken(Compiler* compiler);
bool MatchToken(Compiler* compiler, Token token, TokenType tokenType);
void ConsumeToken(Compiler* compiler, TokenType tokenType, const char* message);
void TryConsumeToken(Compiler* compiler, TokenType tokenType);
bool CheckToken(Compiler* compiler, TokenType tokenType);
#endif
