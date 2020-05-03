#ifndef PyInt_CompilerUpvalue_h
#define PyInt_CompilerUpvalue_h

#include "../../../Headers/Common.h"

typedef struct {
    uint8_t index;
    bool isLocal;
} CompilerUpvalue;

#endif
