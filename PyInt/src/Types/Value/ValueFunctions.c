#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Include our platform compatibility header
#include "platform_compat.h"

#include "ValueFunctions.h"
#include "../../Services/Memory/Memory.h"
#include "../Printing/Printing.h"

void InitValueArray(ValueArray* array) {
	array->values = NULL;
	array->capacity = 0;
	array->count = 0;
}

void WriteValueArray(GarbageCollector* garbageCollector, ValueArray* array, Value value) {
	if (array->capacity < array->count + 1) {
		int oldCapacity = array->capacity;
		array->capacity = GROW_CAPACITY(array->capacity);
        AddTemporary(garbageCollector, value);
		array->values = GROW_ARRAY(garbageCollector, array->values, Value, oldCapacity, array->capacity);
        FreeTemporaries(garbageCollector);
	}
		array->values[array->count] = value;
		array->count++;
}

void FreeValueArray(GarbageCollector* garbageCollector, ValueArray* array) {
	FREE_ARRAY(garbageCollector, Value, array->values, array->capacity);
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
            PYINT_ITOA((int)AS_NUMBER(value), text, 10, 10);
            PYINT_STRCAT(buffer, bufferSize, text);
        }
        else if (printType == STACK) {
            char text[10] = "\0";
            PYINT_ITOA((int)AS_NUMBER(value), text, 10, 10);
            PYINT_STRCAT(buffer, bufferSize, text);
        }
    }
    else if (IS_BOOLEAN(value)) {
        if (printType == OPERAND_VALUE) {
            char text[10] = "\0";
            AS_BOOLEAN(value) ? PYINT_STRCPY(text, 10, "True") : PYINT_STRCPY(text, 10, "False");
            PYINT_STRCAT(buffer, bufferSize, text);
        }
        else if (printType == STACK) {
            char text[10] = "\0";
            AS_BOOLEAN(value) ? PYINT_STRCPY(text, 10, "True") : PYINT_STRCPY(text, 10, "False");
            PYINT_STRCAT(buffer, bufferSize, text);
        }
    }
    else if (IS_CHAR(value)) {
        if (printType == OPERAND_VALUE) {
            PYINT_STRCAT(buffer, bufferSize, "\t\t\t\t");
            PYINT_STRCAT(buffer, bufferSize, &AS_CHAR(value));
        }
        else if (printType == STACK) {
            PYINT_STRCAT(buffer, bufferSize, &AS_CHAR(value));
        }
    }
    else if (IS_NONE(value)) {
        if (printType == OPERAND_VALUE) {
            PYINT_STRCAT(buffer, bufferSize, "None");
        }
        else if (printType == STACK) {
            PYINT_STRCAT(buffer, bufferSize, "None");
        }
    }
    else if (IS_OBJ(value)) {
        PrintObject(settings, value, printType, buffer, bufferSize);
    }
}
