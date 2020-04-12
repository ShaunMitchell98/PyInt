#include <stdio.h>
#include <stdlib.h>
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

void WriteValue(IOSettings* settings, Value value, PrintType printType, char* buffer, int bufferSize) {
    char* formatString;
    if (printType == OPERAND_VALUE) {
        formatString = "\t\t\t\t";
    }
    else if (printType == STACK) {
        formatString="";
    }
    
    if (IS_NUMBER(value)) {
        if (printType == OPERAND_VALUE) {
            char text[10] = "\0";
            _itoa_s(AS_NUMBER(value), text, 10, 10);
            strcat_s(buffer, bufferSize, text);
        }
        else if (printType == STACK) {
            char text[10] = "\0";
            _itoa_s(AS_NUMBER(value), text, 10, 10);
            strcat_s(buffer, bufferSize, text);
        }
    }
    else if (IS_BOOLEAN(value)) {
        if (printType == OPERAND_VALUE) {
            strcat_s(buffer, bufferSize, "\t\t\t\t");
            char text[10] = "\0";
            AS_BOOLEAN(value) ? strcpy_s(text, 10, "True") : strcpy_s(text, 10, "False");
            strcat_s(buffer, bufferSize, text);
        }
        else if (printType == STACK) {
            char text[10] = "\0";
            AS_BOOLEAN(value) ? strcpy_s(text, 10, "True") : strcpy_s(text, 10, "False");
            strcat_s(buffer, bufferSize, text);
        }
    }
    else if (IS_CHAR(value)) {
        if (printType == OPERAND_VALUE) {
            strcat_s(buffer, bufferSize, "\t\t\t\t");
            strcat_s(buffer, bufferSize, &AS_CHAR(value));
        }
        else if (printType == STACK) {
              strcat_s(buffer, bufferSize, &AS_CHAR(value));
        }
    }
    else if (IS_OBJ(value)) {
        PrintObject(settings, value, printType, buffer, bufferSize);
    }
}
