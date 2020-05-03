#include <string.h>

#include "String.h"
#include "../Value/Value.h"
#include "../../Services/Memory/Memory.h"

uint32_t HashString(const char* key, int length) {
    uint32_t hash = 2166136261u;
    for (int i = 0; i < length; i++) {
        hash ^= key[i];
        hash *= 16777619;
    }

    return hash;
}