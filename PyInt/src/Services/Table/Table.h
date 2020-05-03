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

void InitTable(Table* table);
void FreeTable(Table* table);
bool GetTableEntry(Table* table, String* key, Value* value);
bool SetTableEntry(Table* table, String* key, Value value);
bool DeleteTableEntry(Table* table, String* key);
String* FindTableString(Table* table, const char* chars, int length, uint32_t hash);
String* TakeString(Object* heap, Table* table, char* chars, int length);
String* CopyStringToTable(Object* heap, Table* table, const char* chars, int length);
#endif
