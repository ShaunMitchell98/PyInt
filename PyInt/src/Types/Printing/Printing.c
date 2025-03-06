#include <string.h>

// Include our platform compatibility header
#include "platform_compat.h"

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
        PYINT_STRCAT(buffer, bufferSize, AS_CLASS(value)->name->chars);
        break;
    case CLASS_INSTANCE: {
        PYINT_STRCAT(buffer, bufferSize, AS_CLASS_INSTANCE(value)->klass->name->chars);
        PYINT_STRCAT(buffer, bufferSize, " instance");
        break;
    }
    case STRING: {
        if (printType == OPERAND_VALUE) {
            PYINT_STRCAT(buffer, bufferSize, AS_CSTRING(value));
        }
        else if (printType == STACK) {
            PYINT_STRCAT(buffer, bufferSize, AS_CSTRING(value));
        }
        else if (printType == PROGRAM_OUTPUT) {
            PYINT_STRCAT(buffer, bufferSize, AS_CSTRING(value));
            PYINT_STRCAT(buffer, bufferSize, "\n");
        }
        else if (printType == TEST_OUTPUT) {
            PYINT_STRCAT(settings->string, 100, AS_CSTRING(value));
        }
        break;
    }
    case UPVALUE:
        PYINT_STRCAT(buffer, bufferSize, "upvalue");
        break;
    }
}