#include <string.h>

#include "Function.h"
#include "../../Services/Memory/Memory.h"

Function* NewFunction(Object* heap) {
    Function* function = (Function*)AllocateObject(heap, sizeof(Function), FUNCTION);
    function->arity = 0;
    function->upvalueCount = 0;
    function->name = NULL;
    function->hasReturnStatement = false;
    InitBytecode(&function->bytecode);
    return function;
}

void PrintFunction(Function* function, char* buffer, int bufferSize) {
    strcat_s(buffer, bufferSize, "<fn ");
    strcat_s(buffer, bufferSize, function->name->chars);
    strcat_s(buffer, bufferSize, ">");
}