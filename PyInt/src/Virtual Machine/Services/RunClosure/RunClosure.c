#include "RunClosure.h"

#include "../../Run/Run.h"
#include "../../CallFrame/CallFrame.h"

bool RunClosure(VM* vm, Closure* closure, int argCount) {
    CallFrame* frame = &vm->frames[vm->frameCount++];
    frame->closure = closure;
    frame->ip = closure->function->bytecode.code;

    frame->locals = vm->stackTop - argCount - 1;
	return Run(vm);
}