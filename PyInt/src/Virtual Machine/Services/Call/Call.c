#include "Call.h"
#include "../../../Types/NativeFunction/NativeFunction.h"
#include "../Errors/RuntimeError.h"
#include "../Stack/Stack.h"
#include "../../../Types/Class/Class.h"
#include "../../../Types/ClassInstance/ClassInstanceFunctions.h"
#

static bool Call(VM* vm, Closure* closure, int argCount) {
    if (argCount != closure->function->arity) {
        RuntimeError(vm, "Expected %d arguments but got %d.",
            closure->function->arity, argCount);
        return false;
    }

    if (vm->frameCount == FRAMES_MAX) {
        RuntimeError(vm, "Stack overflow.");
        return false;
    }

    CallFrame* frame = &vm->frames[vm->frameCount++];
    frame->closure = closure;
    frame->ip = closure->function->bytecode.code;

    frame->locals = vm->stackTop - argCount - 1;
    return true;
}

bool CallValue(VM* vm, Value callee, int argCount) {
    if (IS_OBJ(callee)) {
        switch (OBJ_TYPE(callee)) {
        case CLASS: {
            Class* klass = AS_CLASS(callee);
            vm->stackTop[-argCount - 1] = OBJ_VAL(NewClassInstance(vm->garbageCollector, klass));
            return true;
        }
        case CLOSURE:
            return Call(vm, AS_CLOSURE(callee), argCount);
        case NATIVE: {
            NativeFn native = AS_NATIVE(callee);
            Value result = native(&vm->settings.output, argCount, vm->stackTop - argCount);
            vm->stackTop -= argCount + 1;
            Push(vm, result);
            return true;
        }
        default:
            //Non-callable object type;
            break;
        }
    }

    RuntimeError(vm, "Can only call functions and classes.");
    return false;
}