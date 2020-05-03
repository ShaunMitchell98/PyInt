#include <stdio.h>
#include <stdarg.h>

#include "RuntimeError.h"
#include "../Stack/Stack.h"
#include "../../CallFrame/CallFrame.h"
#include "../../../Types/Function/Function.h"

void RuntimeError(VM* vm, const char* format) {
    va_list args;
    va_start(args, format);
    vfprintf(stderr, format, args);
    va_end(args);
    fputs("\n", stderr);

    for (int i = 0; i <= vm->frameCount - 1; i++) {
        CallFrame* frame = &vm->frames[i];
        Function* function = frame->closure->function;
        //-1 because the IP is sitting on the next instruction to be executed.
        size_t instruction = frame->ip - function->bytecode.code - 1;
        fprintf(stderr, "[line %d] in ", function->bytecode.lines[instruction]);
        if (function->name == NULL) {
            fprintf(stderr, "script\n");
        }
        else {
            fprintf(stderr, "%s()\n", function->name->chars);
        }
    }

    ResetStack(vm);
}