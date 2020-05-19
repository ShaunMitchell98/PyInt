#include <string.h>

#include "FunctionFunctions.h"
#include "../Object/ObjectFunctions.h"
#include "../../Services/Memory/Memory.h"
#include "../../Services/Bytecode/BytecodeFunctions.h"

Function* NewFunction(GarbageCollector* garbageCollector, FunctionType type) {
    Function* function = (Function*)AllocateObject(garbageCollector, sizeof(Function), FUNCTION);
    function->arity = 0;
    function->upvalueCount = 0;
    function->name = NULL;
    function->type = type;
    function->hasReturnStatement = false;
    InitBytecode(&function->bytecode);
    return function;
}

void PrintFunction(Function* function, char* buffer, int bufferSize, PrintType printType) {
    switch (printType) {
        case GARBAGE_OUTPUT: 
            strcat_s(buffer, bufferSize, "Function");
            break;
        default: {
            strcat_s(buffer, bufferSize, "<fn ");
            strcat_s(buffer, bufferSize, function->name->chars);
            strcat_s(buffer, bufferSize, ">");
            break;
        }
    }
}