#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "../Headers/VM.h"
#include "../Headers/Bytecode.h"
#include "../Headers/Compiler.h"
#include "../Headers/Debug.h"
#include "../Headers/Object.h"
#include "../Headers/InterpreterSettings.h"
#include "../Headers/NativeFunctions.h"
#include "../Headers/VmPrint.h"

static void ResetStack(VM* vm) {
    vm->stackTop = vm->stack;
    vm->frameCount = 0;
}

static void RuntimeError(VM* vm, const char* format) {
    va_list args;
    va_start(args, format);
    vfprintf(stderr, format, args);
    va_end(args);
    fputs("\n", stderr);

    for (int i = 0; i <= vm->frameCount -1 ; i++) {
        CallFrame* frame = &vm->frames[i];
        ObjFunction* function = frame->function;
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

void Push(VM* vm, Value value) {
    *vm->stackTop = value;
    vm->stackTop++;
}

Value Pop(VM* vm) {
    vm->stackTop--;
    return *vm->stackTop;
}

static uint8_t ReadByte(CallFrame* frame) {
    frame->ip++;
    return *(frame->ip - 1);
}

static Value Peek(VM* vm, int i) {
    return *(vm->stackTop - i - 1);
}

static bool Call(VM* vm, ObjFunction* function, int argCount) {
    if (argCount != function->arity) {
        RuntimeError(vm, "Expected %d arguments but got %d.",
            function->arity, argCount);
        return false;
    }

    if (vm->frameCount == FRAMES_MAX) {
        RuntimeError(vm, "Stack overflow.");
        return false;
    }
    
    CallFrame* frame = &vm->frames[vm->frameCount++];
    frame->function = function;
    frame->ip = function->bytecode.code;

    frame->locals = vm->stackTop - argCount - 1;
    return true;
}

static bool CallValue(VM* vm, Value callee, int argCount) {
    if (IS_OBJ(callee)) {
        switch (OBJ_TYPE(callee)) {
        case OBJ_FUNCTION:
            return Call(vm, AS_FUNCTION(callee), argCount);
        case OBJ_NATIVE: {
            NativeFn native = AS_NATIVE(callee);
            Value result = native(vm, argCount, vm->stackTop - argCount);
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

static Value ReadConstant(CallFrame* frame) {
    int address = ReadByte(frame);
    return frame->function->bytecode.constants.values[address];
}

static ObjString* ReadString(CallFrame* frame) {
    int address = ReadByte(frame);
    return AS_STRING(frame->function->bytecode.constants.values[address]);
}

static bool Equal(Value a, Value b) {
    if (IS_CHAR(a) && IS_CHAR(b)) {
        return AS_CHAR(a) == AS_CHAR(b);
    }
    else if (IS_NUMBER(a) && IS_NUMBER(b)) {
        return AS_NUMBER(a) == AS_NUMBER(b);
    }
    else {
        return false;
    }
}

static bool Run(VM* vm) {
    CallFrame* frame = &vm->frames[vm->frameCount-1];

    if (vm->settings.execution.enabled) {
        InitialiseExecutionDisassembly(&vm->settings.execution);
    }

    for (;;) {
        
        if (vm->settings.execution.enabled) {
            DisassembleExecution(vm, &frame->function->bytecode, (int)(frame->ip - frame->function->bytecode.code), vm->stack, vm->stackTop, &vm->settings.execution);
        }

        switch(ReadByte(frame)) {
            case CONSTANT_OP: {
                Push(vm, ReadConstant(frame));
                break;
            }
            case ADD_OP: {
                Value b = Pop(vm);
                Value a = Pop(vm);
                Push(vm, NUMBER_VAL(AS_NUMBER(a) + AS_NUMBER(b)));
                break;
            }
            case SUBTRACT_OP: {
                Value b = Pop(vm);
                Value a = Pop(vm);
                Push(vm, NUMBER_VAL(AS_NUMBER(a) - AS_NUMBER(b)));
                break;
            }
            case MULTIPLY_OP: {
                Value b = Pop(vm);
                Value a = Pop(vm);
                Push(vm, NUMBER_VAL(AS_NUMBER(a) * AS_NUMBER(b)));
                break;
            }
            case DIVIDE_OP: {
                Value b = Pop(vm);
                Value a = Pop(vm);
                Push(vm, NUMBER_VAL(AS_NUMBER(a) / AS_NUMBER(b)));
                break;
            }
            case POWER_OP: {
                Value b = Pop(vm);
                Value a = Pop(vm);
                Push(vm, NUMBER_VAL(pow(AS_NUMBER(a), AS_NUMBER(b))));
                break;
            }
            case GREATER_OP: {
                Value b = Pop(vm);
                Value a = Pop(vm);
                Push(vm, BOOLEAN_VAL(AS_NUMBER(a) > AS_NUMBER(b)));
                break;
            }
            case LESSER_OP: {
                Value b = Pop(vm);
                Value a = Pop(vm);
                Push(vm, BOOLEAN_VAL(AS_NUMBER(a) < AS_NUMBER(b)));
                break;
            }
            case NOT_OP: {
                Push(vm, BOOLEAN_VAL(!AS_BOOLEAN(Pop(vm))));
                break;
            }
            case OR_OP: {
                Value b = Pop(vm);
                Value a = Pop(vm);
                Push(vm,BOOLEAN_VAL(AS_BOOLEAN(a) || AS_BOOLEAN(b)));
                break;
            }
            case AND_OP: {
                Value b = Pop(vm);
                Value a = Pop(vm);
                Push(vm, BOOLEAN_VAL(AS_BOOLEAN(a) && AS_BOOLEAN(b)));
                break;
            }
            case EQUAL_OP: {
                Value b = Pop(vm);
                Value a = Pop(vm);
                bool equal = Equal(a, b);
                Push(vm, BOOLEAN_VAL(equal));
                break;
            }
            case END_OF_ARRAY_OP: {
                ObjString* name = ReadString(frame);
                Value value;
                GetTableEntry(&vm->globals, name, &value);
                ObjString* string = AS_STRING(value);
                Push(vm, BOOLEAN_VAL(vm->arrayIndex == string->length-1));
                break;
            }
            case CALL_OP: {
                int argCount = ReadByte(frame);
                if (!CallValue(vm, Peek(vm, argCount), argCount)) {
                    return INTERPRET_RUNTIME_ERROR;
                }
                frame = &vm->frames[vm->frameCount - 1];
                break;
            }
            case JUMP_IF_FALSE_OP: {
                bool doJump = !AS_BOOLEAN(Pop(vm));
                  frame->ip += 2;
                if (doJump) {
                    frame->ip += (uint16_t)((frame->ip[-2] << 8) | frame->ip[-1]);
                }
                break;
            }
             case JUMP_IF_TRUE_OP: {
                 bool doJump = AS_BOOLEAN(Pop(vm));
                 frame->ip += 2;
                 if (doJump) {
                     frame->ip += (uint16_t)((frame->ip[-2] << 8) | frame->ip[-1]);
                 }
                 break;
             }
             case JUMP_OP: {
                frame->ip += 2;
                frame->ip += (uint16_t)((frame->ip[-2] << 8) | frame->ip[-1]);
                break;
            }
            case LOOP_OP: {
                frame->ip += 2;
                frame->ip -= (uint16_t)((frame->ip[-2] << 8)) |
                frame->ip[-1];
                break;
            }
            case POP_OP: {
                Pop(vm);
                break;
            }
            case SET_GLOBAL_OP: {
                ObjString* name = ReadString(frame);
                SetTableEntry(&vm->globals, name, Peek(vm, 0));
                Pop(vm);
                break;
            }
            case GET_GLOBAL_OP: {
                ObjString* name = ReadString(frame);
                Value value;
                if (GetTableEntry(&vm->globals, name, &value)) {
                    Push(vm, value);
                }
                break;
            }
            case SET_LOCAL_OP: {
                uint8_t slot = ReadByte(frame);
                frame->locals[slot] = Peek(vm, 0);
                break;
            }
            case GET_LOCAL_OP: {
                uint8_t slot = ReadByte(frame);
                Push(vm, frame->locals[slot]);
                break;
            }
            case GET_INDEX_OP: {
                ObjString* name = ReadString(frame);
                Value value;
                GetTableEntry(&vm->globals, name, &value);
                ObjString* string = AS_STRING(value);
                char currentChar = *(string->chars+vm->arrayIndex);
                Push(vm, CHAR_VAL(currentChar));
                vm->arrayIndex++;
                break;
            }
            case RETURN_OP: {
                Value result = Pop(vm);

                vm->frameCount--;
                if (vm->frameCount == 0) {
                    Pop(vm);
                    return false;
                }

                vm->stackTop = frame->locals;
                Push(vm, result);

                frame = &vm->frames[vm->frameCount - 1];
                break;
            }
            case NONE_OP:
                break;
            default:
                return false;
        }
    }
      return true;
}

void InitVM(VM* vm, InterpreterSettings settings) {
    ResetStack(vm);
    vm->arrayIndex = 0;
    vm->settings = settings;
    vm->heap = NULL;
    InitTable(&vm->strings);
    InitTable(&vm->globals);
    DefineStandardFunctions(vm);
}

void FreeVM(VM* vm) {
    ResetStack(vm);
    FreeTable(&vm->strings);
    FreeTable(&vm->globals);
}

InterpretResult Interpret(const char* sourceCode, InterpreterSettings settings) {
    VM vm;
    InitVM(&vm, settings);
    Bytecode bytecode;
    InitBytecode(&bytecode);
    ObjFunction* function = Compile(&vm, &bytecode, sourceCode, settings.runMode);
    
    if (function == NULL) {
        return INTERPRET_COMPILE_ERROR;
    }
    
    Push(&vm, OBJ_VAL(function));
    CallValue(&vm, OBJ_VAL(function), 0);
    
    bool RuntimeError = Run(&vm);
    FreeVM(&vm);
    if (RuntimeError) {
        return INTERPRET_RUNTIME_ERROR;
    }
    else {
        return INTERPRET_OK;
    }
}
