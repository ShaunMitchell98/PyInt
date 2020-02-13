#ifndef PyInt_Scanner_h
#define PyInt_Scanner_h

#include "Token.h"
#include "Common.h"

#define ScanningIndentError "Too many indents"

#define INDENT_STACK_MAX 10

typedef struct {
    const char* path;
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

void InitScanner(const char* sourceCode, const char* fileName);
Token GetToken(void);

#endif
