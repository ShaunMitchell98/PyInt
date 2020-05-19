#ifndef PyInt_ScannerFunctions_h
#define PyInt_ScannerFunctions_h

#include "Scanner.h"
#include "../../Headers/Token.h"

void InitScanner(IndentManager* scanner, const char* sourceCode);
Token GetToken(IndentManager* scanner);

#endif