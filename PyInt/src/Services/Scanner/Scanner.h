#ifndef PyInt_Scanner_h
#define PyInt_Scanner_h

#include "../../Headers/Common.h"
#include "../../Headers/Token.h"

#define ScanningIndentError "Too many indents"

#define INDENT_STACK_MAX 10

typedef struct {
    const char* start;
    const char* current;
    int line;
    bool newLine;
    int indentStack[INDENT_STACK_MAX];
    int stackIndex;
} Scanner;

typedef enum {
    INDENT,
    DEDENT,
    SAME,
    DEDENT_ERROR
} SpaceType;

void InitScanner(Scanner* scanner, const char* sourceCode);
Token GetToken(Scanner* scanner);

#endif
