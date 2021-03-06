#include <string.h>

#include "Table.h"
#include "../../Types/Object/ObjectFunctions.h"
#include "TableFunctions.h"
#include "../Memory/Memory.h"

#define TABLE_MAX_LOAD 0.75

void InitTable(Table* table) {
    table->count = 0;
    table->capacity = 0;
    table->entries = NULL;
}

void FreeTable(GarbageCollector* garbageCollector, Table* table) {
    FREE_ARRAY(garbageCollector, Entry, table->entries, table->capacity);
    InitTable(table);
}

static Entry* FindEntry(Entry* entries, int capacity, String* key) {
    uint32_t index = key->hash % capacity;
    Entry* tombstone = NULL;
    
    for (;;) {
        Entry* entry = &entries[index];
        
        if (entry->key == NULL) {
            if (IS_NONE(entry->value)) {
                return tombstone != NULL ? tombstone : entry;
            }
            else {
                //We found a tombstone
                if (tombstone == NULL) tombstone = entry;
            }
        }
        else if (entry->key == key) {
            //We found the key
            return entry;
        }
        
        index = (index+1) % capacity;
    }
}

bool GetTableEntry(Table* table, String* key, Value* value) {
    if (table->count == 0) return false;
    
    Entry* entry = FindEntry(table->entries, table->capacity, key);
    
    if (entry->key == NULL) return false;
    
    *value = entry->value;
    return true;
}

static void AdjustCapacity(GarbageCollector* garbageCollector, Table* table, int capacity) {
    Entry* entries = ALLOCATE(garbageCollector, Entry, capacity);
    
    for (int i = 0; i < capacity; i++) {
        entries[i].key = NULL;
        entries[i].value = NONE_VAL;    
    }

    for (int i = 0; i < table->capacity; i++) {
        Entry* entry = &table->entries[i];
        if (entry->key == NULL) continue;

        Entry* dest = FindEntry(entries, capacity, entry->key);
        dest->key = entry->key;
        dest->value = entry->value;
    }
    
    FREE_ARRAY(garbageCollector, Entry, table->entries, table->capacity);
    table->entries = entries;
    table->capacity = capacity;
}

bool SetTableEntry(GarbageCollector* garbageCollector, Table* table, String* key, Value value) {
    if (table->count+1 > table->capacity* TABLE_MAX_LOAD) {
        int capacity = GROW_CAPACITY(table->capacity);
        AddTemporary(garbageCollector, OBJ_VAL(key));
        AddTemporary(garbageCollector, value);
        AdjustCapacity(garbageCollector, table, capacity);
        FreeTemporaries(garbageCollector);
    }
    
    Entry* entry = FindEntry(table->entries, table->capacity, key);
    
    bool isNewKey = entry->key == NULL;
    if (isNewKey && IS_NONE(entry->value)) table->count++;
    
    entry->key = key;
    entry->value = value;
    return isNewKey;
}

void TableAddAll(GarbageCollector* garbageCollector, Table* from, Table* to) {
    for (int i = 0; i < from->capacity; i++) {
        Entry* entry = &from->entries[i];
        if (entry != NULL && entry->key != NULL) {
            SetTableEntry(garbageCollector, to, entry->key, entry->value);
        }
    }
}

bool DeleteTableEntry(Table* table, String* key) {
    if (table->count == 0) return false;
    
    //Find the entry;
    Entry* entry = FindEntry(table->entries, table->capacity, key);
    if (entry->key == NULL) return false;
    
    //Place a tombstone in the entry;
    entry->key = NULL;
    entry->value = BOOLEAN_VAL(true);
    
    return true;
}

String* FindTableString(Table* table, const char* chars, int length, uint32_t hash) {
    if (table->count == 0) return NULL;
    
    uint32_t index = hash % table->capacity;
    
    for (;;) {
        Entry* entry = &table->entries[index];
        
        if (entry->key == NULL) {
            //Stop if we find an empty non-tombstone entry.
            if (IS_NONE(entry->value)) return NULL;
        }
        else if (entry->key->length == length && entry->key->hash == hash &&
                 memcmp(entry->key->chars, chars, length) == 0) {
                //We found it.
            return entry->key;
        }
        
        index = (index+1) % table->capacity;
    }
}

static String* AllocateString(GarbageCollector* garbageCollector, Table* table, char* chars, int length, uint32_t hash) {
    String* string = (String*)AllocateObject(garbageCollector, sizeof(String), STRING);
    string->length = length;
    string->chars = chars;
    string->hash = hash;

    SetTableEntry(garbageCollector, table, string, NONE_VAL);

    return string;
}


String* TakeString(GarbageCollector* garbageCollector, Table* table, char* chars, int length) {
    uint32_t hash = HashString(chars, length);

    String* interned = FindTableString(table, chars, length, hash);

    if (interned != NULL) {
        FREE_ARRAY(garbageCollector, char, chars, length + 1);
        return interned;
    }
    return AllocateString(garbageCollector, table, chars, length, hash);
}

String* CopyStringToTable(GarbageCollector* garbageCollector, Table* table, const char* chars, int length) {
    uint32_t hash = HashString(chars, length);

    String* interned = FindTableString(table, chars, length, hash);
    if (interned != NULL) return interned;

    char* heapChars = ALLOCATE(garbageCollector, char, length + 1);
    memcpy(heapChars, chars, length);
    heapChars[length] = '\0';

    return AllocateString(garbageCollector, table, heapChars, length, hash);
}