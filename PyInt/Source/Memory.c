#include <stdlib.h>

#include "../Headers/Common.h"
#include "../Headers/Memory.h"
#include "../Headers/Object.h"

void* Reallocate(void* currentArray, size_t oldSize, size_t newSize) {
	if (newSize == 0) {
		free(currentArray);
		return NULL;
	}
	return realloc(currentArray, newSize);
}

void FreeObject(Obj* object) {
    switch(object->type) {
        case OBJ_FUNCTION: {
            ObjFunction* function = (ObjFunction*)object;
            FreeBytecode(&function->bytecode);
            FREE(ObjFunction, object);
            break;
        }
        case OBJ_NATIVE: {
            FREE(ObjNative, object);
            break;
        }
        case OBJ_STRING: {
            ObjString* string = (ObjString*)object;
            FREE_ARRAY(char, string->chars, string->length+1);
            FREE(ObjString, object);
            break;
        }
    }
}
