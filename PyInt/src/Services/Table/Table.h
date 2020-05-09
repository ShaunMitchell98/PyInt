#ifndef PyInt_Table_h
#define PyInt_Table_h

#include "../../Types/Value/Value.h"
#include "../../Types/String/String.h"

typedef struct {
    String* key;
    Value value;
} Entry;

typedef struct {
    int count;
    int capacity;
    Entry* entries;
} Table;

#endif
