#include <string.h>
#include <stdio.h>

#include "../Headers/Object.h"
#include "../Headers/Value.h"
#include "../Headers/VM.h"
#include "../Headers/Memory.h"
#include "../Headers/HashTable.h"
#include "../Headers/Debug.h"
#include "../Headers/InterpreterSettings.h"

static Obj* AllocateObject(VM* vm, size_t size, ObjType type) {
    Obj* object = (Obj*)Reallocate(NULL, 0, size);
    object->type = type;
    object->next = vm->heap;
    vm->heap = object;
    return object;
}

static ObjString* AllocateString(VM* vm, Table* table,  char* chars, int length, uint32_t hash) {
    ObjString* string = (ObjString*)AllocateObject(vm, sizeof(ObjString), OBJ_STRING);
    string->length = length;
    string->chars = chars;
    string->hash = hash;
    
    SetTableEntry(table, string, NONE_VAL);
    
    return string;
}

static uint32_t HashString(const char* key, int length) {
    uint32_t hash = 2166136261u;
    for (int i = 0; i < length; i++) {
        hash ^= key[i];
        hash *= 16777619;
    }
    
    return hash;
}

ObjString* TakeString(VM* vm, Table* table, char* chars, int length) {
    uint32_t hash = HashString(chars, length);
    
    ObjString* interned = FindTableString(table, chars, length, hash);
    
    if (interned != NULL) {
        FREE_ARRAY(char, chars, length+1);
        return interned;
    }
    return AllocateString(vm, table, chars, length, hash);
}

ObjString* CopyStringToTable(VM* vm, Table* table, const char* chars, int length) {
    uint32_t hash = HashString(chars, length);
    
    ObjString* interned = FindTableString(table, chars, length, hash);
    if (interned != NULL) return interned;
    
    char* heapChars = ALLOCATE(char, length+1);
    memcpy(heapChars, chars, length);
    heapChars[length] = '\0';
    
    return AllocateString(vm, table, heapChars, length, hash);
}

static void PrintFunction(ObjFunction* function, char* buffer, int bufferSize) {
    if (function->name == NULL) {
        strcat_s(buffer, bufferSize, "<script>");
        return;
    }
    printf("<fn %s", function->name->chars);
}

void PrintObject(IOSettings* settings, Value value, PrintType printType, char* buffer, int bufferSize) {
    switch (OBJ_TYPE(value)) {
        case OBJ_FUNCTION:
            PrintFunction(AS_FUNCTION(value), buffer, bufferSize);
            break;
        case OBJ_STRING: {
            if (printType == OPERAND_VALUE) {
                strcat_s(buffer, bufferSize, AS_CSTRING(value));
            }
            else if (printType == STACK) {
                strcat_s(buffer, bufferSize, AS_CSTRING(value));
            }
            else if (printType == PROGRAM_OUTPUT) {
                strcat_s(buffer, bufferSize, AS_CSTRING(value));
                strcat_s(buffer, bufferSize, "\n");
            }
            else if (printType == TEST_OUTPUT) {
                strcat_s(settings->string, 100, AS_CSTRING(value));
            }
            break;
        }
    }
}

ObjFunction* NewFunction(VM* vm) {
    ObjFunction* function = (ObjFunction*)AllocateObject(vm, sizeof(ObjFunction), OBJ_FUNCTION);
    function->arity = 0;
    function->name = NULL;
    function->hasReturnStatement = false;
    InitBytecode(&function->bytecode);
    return function;
}
