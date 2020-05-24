#ifndef PyInt_ScannerFunctions_h
#define PyInt_ScannerFunctions_h

#include "Scanner.h"
#include "../../Headers/Token.h"

void InitScanner(Scanner* scanner, const char* sourceCode);
Token GetToken(Scanner* scanner);

#endif