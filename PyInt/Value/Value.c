#include "Memory.h"
#include "Value.h"
#include <stdio.h>
#include <string.h>
#include "Object.h"

void InitValueArray(ValueArray* array) {
	array->values = NULL;
	array->capacity = 0;
	array->count = 0;
}

void WriteValueArray(ValueArray* array, Value value) {
	if (array->capacity < array->count + 1) {
		int oldCapacity = array->capacity;
		array->capacity = GROW_CAPACITY(array->capacity);
		array->values = GROW_ARRAY(array->values, Value, oldCapacity, array->capacity);
	}
		array->values[array->count] = value;
		array->count++;
}

void FreeValueArray(ValueArray* array) {
	FREE_ARRAY(Value, array->values, array->capacity);
	InitValueArray(array);
}

void PrintValue(Value value, bool tab) {
    char* formatString;
    if (tab) {
        formatString = "\t\t\t\t";
    }
    else {
        formatString="";
    }
    
    if (IS_NUMBER(value)) {
        if (tab) {
            printf("\t\t\t\t%g", AS_NUMBER(value));
        }
        else {
            printf("%g", AS_NUMBER(value));
        }
    }
    else if (IS_BOOLEAN(value)) {
        if (tab) {
            printf("\t\t\t\t%s", AS_BOOLEAN(value) ? "True" : "False");
        }
        else {
            printf("%s", AS_BOOLEAN(value) ? "True" : "False");
        }
    }
    else if (IS_OBJ(value)) {
        PrintObject(value, tab);
    }
}
