#include <stdlib.h>

#include "../../Headers/Common.h"
#include "Memory.h"
#include "../../Types/Closure/Closure.h"
#include "../../Types/Function/Function.h"
#include "../../Types/String/String.h"
#include "../../Types/Upvalue/Upvalue.h"
#include "../../Types/NativeFunction/NativeFunction.h"
#include "../Bytecode/Bytecode.h"

void* Reallocate(void* currentArray, size_t oldSize, size_t newSize) {
	if (newSize == 0) {
		free(currentArray);
		return NULL;
	}
	return realloc(currentArray, newSize);
}

void FreeObject(Object* object) {
    switch(object->type) {
        case CLOSURE: {
            Closure* closure = (Closure*)object;
            FREE_ARRAY(Upvalue*, closure->upvalues, closure->upvalueCount);
            FREE(Closure, object);
        }
        case FUNCTION: {
            Function* function = (Function*)object;
            FreeBytecode(&function->bytecode);
            FREE(Function, object);
            break;
        }
        case NATIVE: {
            FREE(NativeFunction, object);
            break;
        }
        case STRING: {
            String* string = (String*)object;
            FREE_ARRAY(char, string->chars, string->length+1);
            FREE(String, object);
            break;
        }
        case UPVALUE: {
            FREE(Upvalue, object);
            break;
        }
    }
}

void FreeObjects(Object* object) {
    while (object != NULL) {
        Object* next = (Object*) object->next;
        FreeObject(object);
        object = next;
    }
}