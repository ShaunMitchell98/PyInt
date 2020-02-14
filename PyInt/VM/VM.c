#include <stdio.h>
#include <math.h>

#include "VM.h"
#include "Bytecode.h"
#include "Compiler.h"
#include "Debug.h"
#include "Object.h"

VM vm;

static void ResetStack() {
    vm.stackTop = vm.stack;
}

static void Push(Value value) {
    *vm.stackTop = value;
    vm.stackTop++;
}

static Value Pop() {
    vm.stackTop--;
    return * vm.stackTop;
}

static uint8_t ReadByte() {
    vm.ip++;
    return *(vm.ip-1);
}

static Value Peek(int i) {
    return *(vm.stackTop - i);
}

static Value ReadConstant() {
    int address = ReadByte();
    return vm.bytecode->constants.values[address];
}

static ObjString* ReadString() {
    int address = ReadByte();
    return AS_STRING(vm.bytecode->constants.values[address]);
}

static bool Run() {
#ifdef DEBUG_TRACE_EXECUTION
    DisassembleBytecode(vm.bytecode);
    printf("Program execution \n");
    printf("Instruction Address   Line  Instruction   Operand address    Operand value   Stack      \n");
#endif
    for (;;) {
        
#ifdef DEBUG_TRACE_EXECUTION
        DisassembleExecution(vm.bytecode, (int)(vm.ip-vm.bytecode->code), &vm.stack, vm.stackTop);
#endif
        switch(ReadByte()) {
            case CONSTANT_OP: {
                Push(ReadConstant());
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
                Push(BOOLEAN_VAL(AS_BOOLEAN(a) || AS_BOOLEAN(b)));
                break;
            }
            case PRINT_OP: {
                Value a = Pop();
                if (IS_NUMBER(a)) {
                    printf("%f\n", AS_NUMBER(a));
                }
                else if (IS_BOOLEAN(a)) {
                    printf(AS_BOOLEAN(a) ? "True\n" : "False\n");
                }
                else {
                    return false;
                }
                break;
            }
            case JUMP_IF_FALSE_OP: {
                bool doJump = !AS_BOOLEAN(Pop());
                  vm.ip += 2;
                if (doJump) {
                    vm.ip += (uint16_t)((vm.ip[-2] << 8) | vm.ip[-1]);
                }
                break;
            }
            case JUMP_OP: {
                vm.ip += 2;
                vm.ip += (uint16_t)((vm.ip[-2] << 8) | vm.ip[-1]);
                break;
            }
            case LOOP_OP: {
                vm.ip += 2;
                vm.ip -= (uint16_t)((vm.ip[-2] << 8)) |
                vm.ip[-1];
                break;
            }
            case POP_OP: {
                Pop();
                break;
            }
            case DEFINE_GLOBAL_OP: {
                ObjString* name = ReadString();
                SetTableEntry(&vm.globals, name, NONE_VAL);
                break;
            }
            case SET_GLOBAL_OP: {
                ObjString* name = ReadString();
                SetTableEntry(&vm.globals, name, Pop());
                break;
            }
            case GET_GLOBAL_OP: {
                ObjString* name = ReadString();
                Value value;
                if (GetTableEntry(&vm.globals, name, &value)) {
                    Push(value);
                }
                break;
            }
            case SET_LOCAL_OP: {
                uint8_t slot = ReadByte();
                vm.stack[slot] = Peek(1);
                break;
            }
            case GET_LOCAL_OP: {
                uint8_t slot = ReadByte();
                Push(vm.stack[slot]);
                break;
            }
            case DECLARE_LOCAL_OP: {
                uint8_t slot = ReadByte();
                vm.stack[slot] = NONE_VAL;
                break;
            }
            case RETURN_OP: {
                //Do nothing;
                return true;
            }
            case NONE_OP:
                break;
            default:
                return false;
        }
    }
      return true;
}


void InitVM() {
    ResetStack();
    InitTable(&vm.strings);
    InitTable(&vm.globals);
}

void FreeVM() {
    ResetStack();
    FreeTable(&vm.strings);
    FreeTable(&vm.globals);
}

InterpretResult Interpret(const char* sourceCode, const char* path) {
    InitVM();
    Bytecode bytecode;
    InitBytecode(&bytecode);
    bool compileTimeError = Compile(&bytecode, sourceCode, path);
    
    if (compileTimeError) {
        return INTERPRET_COMPILE_ERROR;
    }
    
    vm.bytecode = &bytecode;
    vm.ip = bytecode.code;
    
    bool RuntimeError = Run();
    FreeVM();
    if (RuntimeError) {
        return INTERPRET_RUNTIME_ERROR;
    }
    else {
        return INTERPRET_OK;
    }
}
