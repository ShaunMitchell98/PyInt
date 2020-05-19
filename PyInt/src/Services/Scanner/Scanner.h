#ifndef PyInt_Scanner_h
#define PyInt_Scanner_h

#include "../../Headers/Common.h"
#include "../../Headers/Token.h"
#include "IndentManager/IndentManager.h"

typedef struct {
    const char* start;
    const char* current;
    int line;
    IndentManager indentManager;
} Scanner;

#endif
