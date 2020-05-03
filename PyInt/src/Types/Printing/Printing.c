#include <string.h>

#include "Printing.h"
#include "../Object/Object.h"
#include "../Function/Function.h"
#include "../NativeFunction/NativeFunction.h"
#include "../String/String.h"
#include "../Closure/Closure.h"

void PrintObject(IOSettings* settings, Value value, PrintType printType, char* buffer, int bufferSize) {
    switch (OBJ_TYPE(value)) {
    case CLOSURE:
        PrintFunction(AS_CLOSURE(value)->function, buffer, bufferSize);
        break;
    case FUNCTION:
        PrintFunction(AS_FUNCTION(value), buffer, bufferSize);
        break;
    case NATIVE:
        PrintNativeFunction(((NativeFunction*)AS_OBJ(value))->name, buffer, bufferSize);
        break;
    case STRING: {
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
    case UPVALUE:
        strcat_s(buffer, bufferSize, "upvalue");
        break;
    }
}