#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "../Headers/VM.h"
#include "../Headers/Bytecode.h"
#include "../Headers/Compiler.h"
#include "../Headers/Debug.h"
#include "../Headers/Object.h"

VM vm;

static void ResetStack() {
    vm.stackTop = vm.stack;
    vm.frameCount = 0;
}

static void Push(Value value) {
    *vm.stackTop = value;
    vm.stackTop++;
}

static Value Pop() {
    vm.stackTop--;
    return *vm.stackTop;
}

static uint8_t ReadByte(CallFrame* frame) {
    frame->ip++;
    return *(frame->ip - 1);
}

static Value Peek(int i) {
    return *(vm.stackTop - i);
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

static bool PrintValueToTerminal(Value a) {
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
        PrintObject(a, PROGRAM_OUTPUT);
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

static bool PrintValueToFile(Value a) {
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
        PrintObject(a, PROGRAM_OUTPUT);
    }
    else {
        return false;
    }
    WriteToFile(text, vm.printInfo.filePath);
    return true;
}

static bool PrintValueToString(Value a) {
    if (IS_NUMBER(a)) {
        _itoa_s(AS_NUMBER(a), vm.printInfo.output, 100, 10);
    }
    else if (IS_BOOLEAN(a)) {
        AS_BOOLEAN(a) == true ? strcat_s(vm.printInfo.output, 100, "true") : strcat_s(vm.printInfo.output, 100, "false");
    }
    else if (IS_CHAR(a)) {
        strcat_s(vm.printInfo.output, 100, &AS_CHAR(a));
    }
    else if (IS_OBJ(a)) {
        PrintObject(a, PROGRAM_OUTPUT);
    }
    else {
        return false;
    }
    return true;
}

static bool Run() {
    CallFrame* frame = &vm.frames[vm.frameCount-1];
    
#ifdef DEBUG_TRACE_EXECUTION
    DisassembleBytecode(&frame->function->bytecode);
    printf("Program execution \n");
    printf("Instruction Address   Line  Instruction   Operand address    Operand value   Stack      \n");
#endif
    for (;;) {
        
#ifdef DEBUG_TRACE_EXECUTION
        DisassembleExecution(&frame->function->bytecode, (int)(frame->ip-frame->function->bytecode.code), vm.stack, vm.stackTop);
#endif
        switch(ReadByte(frame)) {
            case CONSTANT_OP: {
                Push(ReadConstant(frame));
                break;
            }
            case ADD_OP: {
                Value b = Pop();
                Value a = Pop();
                Push(NUMBER_VAL(AS_NUMBER(a) + AS_NUMBER(b)));
                break;
            }
            case SUBTRACT_OP: {
                Value b = Pop();
                Value a = Pop();
                Push(NUMBER_VAL(AS_NUMBER(a) - AS_NUMBER(b)));
                break;
            }
            case MULTIPLY_OP: {
                Value b = Pop();
                Value a = Pop();
                Push(NUMBER_VAL(AS_NUMBER(a) * AS_NUMBER(b)));
                break;
            }
            case DIVIDE_OP: {
                Value b = Pop();
                Value a = Pop();
                Push(NUMBER_VAL(AS_NUMBER(a) / AS_NUMBER(b)));
                break;
            }
            case POWER_OP: {
                Value b = Pop();
                Value a = Pop();
                Push(NUMBER_VAL(pow(AS_NUMBER(a), AS_NUMBER(b))));
                break;
            }
            case GREATER_OP: {
                Value b = Pop();
                Value a = Pop();
                Push(BOOLEAN_VAL(AS_NUMBER(a) > AS_NUMBER(b)));
                break;
            }
            case LESSER_OP: {
                Value b = Pop();
                Value a = Pop();
                Push(BOOLEAN_VAL(AS_NUMBER(a) < AS_NUMBER(b)));
                break;
            }
            case NOT_OP: {
                Push(BOOLEAN_VAL(!AS_BOOLEAN(Pop())));
                break;
            }
            case OR_OP: {
                Value b = Pop();
                Value a = Pop();
                Push(BOOLEAN_VAL(AS_BOOLEAN(a) || AS_BOOLEAN(b)));
                break;
            }
            case AND_OP: {
                Value b = Pop();
                Value a = Pop();
                Push(BOOLEAN_VAL(AS_BOOLEAN(a) && AS_BOOLEAN(b)));
                break;
            }
            case EQUAL_OP: {
                Value b = Pop();
                Value a = Pop();
                bool equal = Equal(a, b);
                Push(BOOLEAN_VAL(equal));
                break;
            }
            case END_OF_ARRAY_OP: {
                ObjString* name = ReadString(frame);
                Value value;
                GetTableEntry(&vm.globals, name, &value);
                ObjString* string = AS_STRING(value);
                Push(BOOLEAN_VAL(vm.arrayIndex == string->length-1));
                break;
            }
            case PRINT_OP: {
                Value a = Pop();
                if (vm.printInfo.printLocation == PRINT_TERMINAL) {
                    bool success = PrintValueToTerminal(a);
                    if (!success) return true;
                }
                else if (vm.printInfo.printLocation == PRINT_FILE) {
                    bool success = PrintValueToFile(a);
                    if (!success) return true;
                }
                else if (vm.printInfo.printLocation == PRINT_STRING) {
                    bool success = PrintValueToString(a);
                    if (!success) return true;
                }
                break;
            }
            case JUMP_IF_FALSE_OP: {
                bool doJump = !AS_BOOLEAN(Pop());
                  frame->ip += 2;
                if (doJump) {
                    frame->ip += (uint16_t)((frame->ip[-2] << 8) | frame->ip[-1]);
                }
                break;
            }
             case JUMP_IF_TRUE_OP: {
                 bool doJump = AS_BOOLEAN(Pop());
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
                Pop();
                break;
            }
            case DEFINE_GLOBAL_OP: {
                ObjString* name = ReadString(frame);
                SetTableEntry(&vm.globals, name, NONE_VAL);
                break;
            }
            case SET_GLOBAL_OP: {
                ObjString* name = ReadString(frame);
                SetTableEntry(&vm.globals, name, Pop());
                break;
            }
            case GET_GLOBAL_OP: {
                ObjString* name = ReadString(frame);
                Value value;
                if (GetTableEntry(&vm.globals, name, &value)) {
                    Push(value);
                }
                break;
            }
            case SET_LOCAL_OP: {
                uint8_t slot = ReadByte(frame);
                frame->locals[slot] = Peek(1);
                break;
            }
            case GET_LOCAL_OP: {
                uint8_t slot = ReadByte(frame);
                Push(frame->locals[slot]);
                break;
            }
            case DECLARE_LOCAL_OP: {
                uint8_t slot = ReadByte(frame);
                vm.stack[slot] = NONE_VAL;
                break;
            }
            case GET_INDEX_OP: {
                ObjString* name = ReadString(frame);
                Value value;
                GetTableEntry(&vm.globals, name, &value);
                ObjString* string = AS_STRING(value);
                char currentChar = *(string->chars+vm.arrayIndex);
                Push(CHAR_VAL(currentChar));
                vm.arrayIndex++;
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

void InitVM(PrintInfo printInfo) {
    ResetStack();
    vm.arrayIndex = 0;
    vm.printInfo = printInfo;
    InitTable(&vm.strings);
    InitTable(&vm.globals);
}

void FreeVM() {
    ResetStack();
    FreeTable(&vm.strings);
    FreeTable(&vm.globals);
}

InterpretResult Interpret(const char* sourceCode, const char* path, PrintInfo printInfo) {
    InitVM(printInfo);
    Bytecode bytecode;
    InitBytecode(&bytecode);
    ObjFunction* function = Compile(&bytecode, sourceCode, path);
    
    if (function == NULL) {
        return INTERPRET_COMPILE_ERROR;
    }
    
    Push(OBJ_VAL(function));
    CallFrame* frame = &vm.frames[vm.frameCount++];
    frame->function = function;
    frame->ip = function->bytecode.code;
    frame->locals = vm.stack;
    
    bool RuntimeError = Run();
    FreeVM();
    if (RuntimeError) {
        return INTERPRET_RUNTIME_ERROR;
    }
    else {
        return INTERPRET_OK;
    }
}
