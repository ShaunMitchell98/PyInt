#ifndef PyInt_HashTable_h
#define PyInt_HashTable_h

#include "Value.h"

typedef struct {
    ObjString* key;
    Value value;
} Entry;

typedef struct {
    int count;
    int capacity;
    Entry* entries;
} Table;

void InitTable(Table* table);
void FreeTable(Table* table);
bool GetTableEntry(Table* table, ObjString* key, Value* value);
bool SetTableEntry(Table* table, ObjString* key, Value value);
bool DeleteTableEntry(Table* table, ObjString* key);
ObjString* FindTableString(Table* table, const char* chars, int length, uint32_t hash);
#endif
