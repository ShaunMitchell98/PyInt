#ifndef PyInt_Local_h
#define PyInt_Local_h

#include "../../../Headers/Token.h"
#include "../../../Headers/Common.h"

typedef struct {
    bool isCaptured;
    Token name;
} Local;

#endif