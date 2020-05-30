#include "Call.h"
#include "../../../Types/NativeFunction/NativeFunction.h"
#include "../Errors/RuntimeError.h"
#include "../Stack/Stack.h"
#include "../../../Types/BoundMethod/BoundMethod.h"
#include "../../../Types/Class/Class.h"
#include "../../../Types/ClassInstance/ClassInstanceFunctions.h"
#include "../../../Services/Table/TableFunctions.h"
#include "../RunClosure/RunClosure.h"
#

bool Call(VM* vm, Closure* closure, int argCount) {
    if (argCount != closure->function->arity) {
        RuntimeError(vm, "Expected %d arguments but got %d.",
            closure->function->arity, argCount);
        return false;
    }

    if (vm->frameCount == FRAMES_MAX) {
        RuntimeError(vm, "Stack overflow.");
        return false;
    }

    return RunClosure(vm, closure, argCount);
}

bool CallValue(VM* vm, Value callee, int argCount) {
    if (IS_OBJ(callee)) {
        switch (OBJ_TYPE(callee)) {
        case BOUND_METHOD: {
            BoundMethod* boundMethod = AS_BOUND_METHOD(callee);
            vm->stackTop[-argCount - 1] = boundMethod->receiver;
            return Call(vm, boundMethod->method, argCount);
        }
        case CLASS: {
            Class* klass = AS_CLASS(callee);
            vm->stackTop[-argCount-1] = OBJ_VAL(NewClassInstance(vm->garbageCollector, klass));
            Value initialiser;
            RunClosure(vm, klass->init, 0);
            if (GetTableEntry(&klass->methods, vm->garbageCollector->initString, &initialiser)) {
                return Call(vm, AS_CLOSURE(initialiser), argCount);
            }
            else if (argCount != 0) {
                RuntimeError(vm, "Expected 0 arguements but got %d.", argCount);
                return true;
            }
            return false;
        }
        case CLOSURE:
            return Call(vm, AS_CLOSURE(callee), argCount);
        case NATIVE: {
            NativeFn native = AS_NATIVE(callee);
            Value result = native(&vm->settings.output, argCount, vm->stackTop - argCount);
            vm->stackTop -= argCount + 1;
            Push(vm, result);
            return false;
        }
        default:
            //Non-callable object type;
            break;
        }
    }

    RuntimeError(vm, "Can only call functions and classes.");
    return false;
}