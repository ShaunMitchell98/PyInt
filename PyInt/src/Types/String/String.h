#ifndef PyInt_String_h
#define PyInt_String_h

#include "../../Headers/Common.h"
#include "../Object/Object.h"

typedef struct {
    Object obj;
    int length;
    char* chars;
    uint32_t hash;
} String;

#define AS_STRING(value) ((String*)AS_OBJ(value))
#define AS_CSTRING(value) (((String*)AS_OBJ(value))->chars)

uint32_t HashString(const char* key, int length);

#endif