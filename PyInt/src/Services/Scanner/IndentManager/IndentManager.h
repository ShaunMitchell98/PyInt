#ifndef PyInt_IndentManager_h
#define PyInt_IndentManager_h

#define ScanningIndentError "Too many indents"

#define INDENT_STACK_MAX 10

typedef enum {
    INDENT,
    DEDENT,
    SAME,
    DEDENT_ERROR
} IndentType;

typedef struct {
    bool newLine;
    int indentStack[INDENT_STACK_MAX];
    int indentStackIndex;
    int spaceCount;
} IndentManager;

#endif