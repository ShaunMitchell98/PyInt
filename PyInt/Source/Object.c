#include <string.h>
#include <stdio.h>

#include "../Headers/Object.h"
#include "../Headers/Value.h"
#include "../Headers/VM.h"
#include "../Headers/Memory.h"
#include "../Headers/HashTable.h"
#include "../Headers/Debug.h"

#define ALLOCATE_OBJ(type, objectType) \
    (type*)AllocateObject(sizeof(type), objectType)

static Obj* AllocateObject(size_t size, ObjType type) {
    Obj* object = (Obj*)Reallocate(NULL, 0, size);
    object->type = type;
    object->next = vm.heap;
    vm.heap = object;
    return object;
}

static ObjString* AllocateString(char* chars, int length, uint32_t hash) {
    ObjString* string = ALLOCATE_OBJ(ObjString, OBJ_STRING);
    string->length = length;
    string->chars = chars;
    string->hash = hash;
    
    SetTableEntry(&vm.strings, string, NONE_VAL);
    
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

ObjString* TakeString(char* chars, int length) {
    uint32_t hash = HashString(chars, length);
    
    ObjString* interned = FindTableString(&vm.strings, chars, length, hash);
    
    if (interned != NULL) {
        FREE_ARRAY(char, chars, length+1);
        return interned;
    }
    return AllocateString(chars, length, hash);
}

ObjString* CopyString(const char* chars, int length) {
    uint32_t hash = HashString(chars, length);
    
    ObjString* interned = FindTableString(&vm.strings, chars, length, hash);
    if (interned != NULL) return interned;
    
    char* heapChars = ALLOCATE(char, length+1);
    memcpy(heapChars, chars, length);
    heapChars[length] = '\0';
    
    return AllocateString(heapChars, length, hash);
}

static void PrintFunction(ObjFunction* function) {
    if (function->name == NULL) {
        printf("<script>");
        return;
    }
    printf("<fn %s", function->name->chars);
}

void PrintObject(Value value, PrintType printType) {
    switch (OBJ_TYPE(value)) {
        case OBJ_FUNCTION:
            PrintFunction(AS_FUNCTION(value));
            break;
        case OBJ_STRING: {
            if (printType == OPERAND_VALUE) {
                 printf("\t\t\t%s", AS_CSTRING(value));
            }
            else if (printType == STACK) {
                 printf("%s", AS_CSTRING(value));
            }
            else if (printType == PROGRAM_OUTPUT) {
                printf("%s\n", AS_CSTRING(value));
            }
            break;
        }
    }
}

ObjFunction* NewFunction() {
    ObjFunction* function = ALLOCATE_OBJ(ObjFunction, OBJ_FUNCTION);
    function->arity = 0;
    function->name = NULL;
    InitBytecode(&function->bytecode);
    return function;
}
