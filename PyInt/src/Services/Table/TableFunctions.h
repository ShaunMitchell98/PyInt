#ifndef PyInt_TableFunctions_h
#define PyInt_TableFunctions_h

#include "Table.h"
#include "../../Virtual Machine/Settings/Settings.h"
#include "../GarbageCollection/GarbageCollector/GarbageCollector.h"

void InitTable(Table* table);
void FreeTable(GarbageCollector* garbageCollector, Table* table);
bool GetTableEntry(Table* table, String* key, Value* value);
bool SetTableEntry(GarbageCollector* garbageCollector, Table* table, String* key, Value value);
bool DeleteTableEntry(Table* table, String* key);
String* FindTableString(Table* table, const char* chars, int length, uint32_t hash);
String* TakeString(GarbageCollector* garbageCollector, Table* table, char* chars, int length);
String* CopyStringToTable(GarbageCollector* garbageCollector, Table* table, const char* chars, int length);

#endif