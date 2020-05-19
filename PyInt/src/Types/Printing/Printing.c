#include <string.h>

#include "Printing.h"
#include "../Object/Object.h"
#include "../Function/FunctionFunctions.h"
#include "../NativeFunction/NativeFunction.h"
#include "../NativeFunction/NativeFunctionFunctions.h"
#include "../String/String.h"
#include "../Closure/Closure.h"
#include "../Class/Class.h"
#include "../ClassInstance/ClassInstance.h"
#include "../BoundMethod/BoundMethod.h"

void PrintObject(IOSettings* settings, Value value, PrintType printType, char* buffer, int bufferSize) {
    switch (OBJ_TYPE(value)) {
    case BOUND_METHOD: 
        PrintFunction(AS_BOUND_METHOD(value)->method->function, buffer, bufferSize, printType);
        break;
    case CLOSURE:
        PrintFunction(AS_CLOSURE(value)->function, buffer, bufferSize, printType);
        break;
    case FUNCTION:
        PrintFunction(AS_FUNCTION(value), buffer, bufferSize, printType);
        break;
    case NATIVE:
        PrintNativeFunction(((NativeFunction*)AS_OBJ(value))->name, buffer, bufferSize);
        break;
    case CLASS:
        strcat_s(buffer, bufferSize, AS_CLASS(value)->name->chars);
        break;
    case CLASS_INSTANCE: {
        strcat_s(buffer, bufferSize, AS_CLASS_INSTANCE(value)->klass->name->chars);
        strcat_s(buffer, bufferSize, " instance");
        break;
    }
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