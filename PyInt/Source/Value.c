#include <stdio.h>
#include <string.h>

#include "../Headers/Memory.h"
#include "../Headers/Value.h"
#include "../Headers/Debug.h"
#include "../Headers/Object.h"

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

void PrintValue(Value value, PrintType printType) {
    char* formatString;
    if (printType == OPERAND_VALUE) {
        formatString = "\t\t\t\t";
    }
    else if (printType == STACK) {
        formatString="";
    }
    
    if (IS_NUMBER(value)) {
        if (printType == OPERAND_VALUE) {
            printf("\t\t\t\t%g", AS_NUMBER(value));
        }
        else if (printType == STACK) {
            printf("%g", AS_NUMBER(value));
        }
    }
    else if (IS_BOOLEAN(value)) {
        if (printType == OPERAND_VALUE) {
            printf("\t\t\t\t%s", AS_BOOLEAN(value) ? "True" : "False");
        }
        else if (printType == STACK) {
            printf("%s", AS_BOOLEAN(value) ? "True" : "False");
        }
    }
    else if (IS_CHAR(value)) {
        if (printType == OPERAND_VALUE) {
            printf("\t\t\t\t%c", AS_CHAR(value));
        }
        else if (printType == STACK) {
              printf("%c", AS_CHAR(value));
        }
    }
    else if (IS_OBJ(value)) {
        PrintObject(value, printType);
    }
}
