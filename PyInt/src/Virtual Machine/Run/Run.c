#include <math.h>

#include "Run.h"
#include "../Services/Stack/Stack.h"
#include "../../Services/Disassembly/ExecutionDisassembly/ExecutionDisassembly.h"
#include "../Services/Call/Call.h"
#include "../Services/Upvalues/Upvalues.h"

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

bool Run(VM* vm) {
    CallFrame* frame = &vm->frames[vm->frameCount - 1];

    if (vm->settings.execution.enabled) {
        InitialiseExecutionDisassembly(&vm->settings.execution);
    }

    for (;;) {

        if (vm->settings.execution.enabled) {
            DisassembleExecution(&frame->closure->function->bytecode, (int)(frame->ip - frame->closure->function->bytecode.code), frame->locals, vm->stackTop, &vm->settings.execution);
        }

        switch (ReadByte(frame)) {
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
            Push(vm, BOOLEAN_VAL(AS_BOOLEAN(a) || AS_BOOLEAN(b)));
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
            String* name = ReadString(frame);
            Value value;
            GetTableEntry(&vm->globals, name, &value);
            String* string = AS_STRING(value);
            Push(vm, BOOLEAN_VAL(vm->arrayIndex == string->length - 1));
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
        case CLOSURE_OP: {
            Function* function = AS_FUNCTION(ReadConstant(frame));
            Closure* closure = NewClosure(vm->heap, function);
            Push(vm, OBJ_VAL(closure));
            for (int i = 0; i < closure->upvalueCount; i++) {
                uint8_t isLocal = ReadByte(frame);
                uint8_t index = ReadByte(frame);
                if (isLocal) {
                    closure->upvalues[i] = CaptureUpvalue(vm, frame->locals + index);
                }
                else {
                    closure->upvalues[i] = frame->closure->upvalues[index];
                }
            }
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
            String* name = ReadString(frame);
            SetTableEntry(&vm->globals, name, Peek(vm, 0));
            Pop(vm);
            break;
        }
        case GET_GLOBAL_OP: {
            String* name = ReadString(frame);
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
        case SET_UPVALUE_OP: {
            uint8_t slot = ReadByte(frame);
            *frame->closure->upvalues[slot]->location = Peek(vm, 0);
            break;
        }
        case GET_UPVALUE_OP: {
            uint8_t slot = ReadByte(frame);
            Push(vm, *frame->closure->upvalues[slot]->location);
            break;
        }
        case GET_INDEX_OP: {
            String* name = ReadString(frame);
            Value value;
            GetTableEntry(&vm->globals, name, &value);
            String* string = AS_STRING(value);
            char currentChar = *(string->chars + vm->arrayIndex);
            Push(vm, CHAR_VAL(currentChar));
            vm->arrayIndex++;
            break;
        }
        case CLOSE_UPVALUE_OP: {
            CloseUpvalues(vm, vm->stackTop - 1);
            Pop(vm);
            break;
        }
        case RETURN_OP: {
            Value result = Pop(vm);

            CloseUpvalues(vm, frame->locals);

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