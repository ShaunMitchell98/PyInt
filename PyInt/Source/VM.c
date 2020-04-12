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

static void ResetStack(VM* vm) {
    vm->stackTop = vm->stack;
    vm->frameCount = 0;
}

static void Push(VM* vm, Value value) {
    *vm->stackTop = value;
    vm->stackTop++;
}

static Value Pop(VM* vm) {
    vm->stackTop--;
    return *vm->stackTop;
}

static uint8_t ReadByte(CallFrame* frame) {
    frame->ip++;
    return *(frame->ip - 1);
}

static Value Peek(VM* vm, int i) {
    return *(vm->stackTop - i);
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

static bool PrintValueToTerminal(VM* vm, Value a) {
    if (IS_NUMBER(a)) {
        printf("%f\n", AS_NUMBER(a));
    }
    else if (IS_BOOLEAN(a)) {
        printf(AS_BOOLEAN(a) ? "True\n" : "False\n");
    }
    else if (IS_CHAR(a)) {
        printf("%c", AS_CHAR(a));
    }
    else if (IS_OBJ(a)) {
        char* buffer = (char*)malloc(100 * sizeof(char));
        PrintObject(&vm->settings.output, a, PROGRAM_OUTPUT, buffer, 100);
        printf(buffer);
        free(buffer);
    }
    else {
        return false;
    }
    return true;
}


static bool WriteToFile(const char* text, const char* filePath) {
    FILE file;
    FILE* fp = &file;
    errno_t err = fopen_s(&fp, filePath, "wb");

    if (err != 0) {
        fprintf(stderr, "Cannot write to file %s", filePath);
        return false;
    }

    fputs(text, fp);
    fclose(fp);
    return true;
}

static bool PrintValueToFile(VM* vm, Value a) {
    char text[10];
    if (IS_NUMBER(a)) {
        _itoa_s(AS_NUMBER(a), text, 10, 10);
    }
    else if (IS_BOOLEAN(a)) {
        AS_BOOLEAN(a) == true ? strcpy_s(text, 10, "true") : strcpy_s(text, 10, "false");
    }
    else if (IS_CHAR(a)) {
        strcpy_s(text, 10, &AS_CHAR(a));
    }
    else if (IS_OBJ(a)) {
        PrintObject(&vm->settings.output, a, PROGRAM_OUTPUT, text, 10);
    }
    else {
        return false;
    }
    WriteToFile(text, vm->settings.output.filePath);
    return true;
}

static bool PrintValueToString(VM* vm, Value a) {
    if (IS_NUMBER(a)) {
        char numOutput[10] = "";
        _itoa_s(AS_NUMBER(a), numOutput, 10, 10);
        strcat_s(vm->settings.output.string, 100, numOutput);
    }
    else if (IS_BOOLEAN(a)) {
        AS_BOOLEAN(a) == true ? strcat_s(vm->settings.output.string, 100, "true") : strcat_s(vm->settings.output.string, 100, "false");
    }
    else if (IS_CHAR(a)) {
        strcat_s(vm->settings.output.string, 100, &AS_CHAR(a));
    }
    else if (IS_OBJ(a)) {
        PrintObject(&vm->settings.output, a, TEST_OUTPUT, vm->settings.output.string, 100);
    }
    else {
        return false;
    }
    return true;
}

static bool Run(VM* vm) {
    CallFrame* frame = &vm->frames[vm->frameCount-1];
    
    if (vm->settings.bytecode.enabled) {
        DisassembleBytecode(vm, &frame->function->bytecode, &vm->settings.bytecode);
    }

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
            case PRINT_OP: {
                Value a = Pop(vm);
                if (vm->settings.output.location == LOCATION_TERMINAL) {
                    bool success = PrintValueToTerminal(vm, a);
                    if (!success) return true;
                }
                else if (vm->settings.output.location == LOCATION_FILE) {
                    bool success = PrintValueToFile(vm, a);
                    if (!success) return true;
                }
                else if (vm->settings.output.location == LOCATION_STRING) {
                    bool success = PrintValueToString(vm, a);
                    if (!success) return true;
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
            case DEFINE_GLOBAL_OP: {
                ObjString* name = ReadString(frame);
                SetTableEntry(&vm->globals, name, NONE_VAL);
                break;
            }
            case SET_GLOBAL_OP: {
                ObjString* name = ReadString(frame);
                SetTableEntry(&vm->globals, name, Pop(vm));
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
                frame->locals[slot] = Peek(vm, 1);
                break;
            }
            case GET_LOCAL_OP: {
                uint8_t slot = ReadByte(frame);
                Push(vm, frame->locals[slot]);
                break;
            }
            case DECLARE_LOCAL_OP: {
                uint8_t slot = ReadByte(frame);
                vm->stack[slot] = NONE_VAL;
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
                //Do nothing;
                return false;
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
    InitTable(&vm->strings);
    InitTable(&vm->globals);
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
    ObjFunction* function = Compile(&vm, &bytecode, sourceCode, settings.output.filePath);
    
    if (function == NULL) {
        return INTERPRET_COMPILE_ERROR;
    }
    
    Push(&vm, OBJ_VAL(function));
    CallFrame* frame = &vm.frames[vm.frameCount++];
    frame->function = function;
    frame->ip = function->bytecode.code;
    frame->locals = vm.stack;
    
    bool RuntimeError = Run(&vm);
    FreeVM(&vm);
    if (RuntimeError) {
        return INTERPRET_RUNTIME_ERROR;
    }
    else {
        return INTERPRET_OK;
    }
}
