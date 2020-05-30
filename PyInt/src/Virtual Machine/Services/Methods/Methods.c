#include "Methods.h"
#include "../../../Types/Value/Value.h"
#include "../Stack/Stack.h"
#include "../../../Types/Class/Class.h"
#include "../../../Services/Table/TableFunctions.h"
#include "../../../Types/BoundMethod//BoundMethodFunctions.h"
#include "../../../Types/ClassInstance/ClassInstance.h"
#include "../Call/Call.h"
#include "../Errors/RuntimeError.h"

bool InvokeFromClass(VM* vm, Class* klass, String* name, int argCount) {
	Value method;
	if (!GetTableEntry(&klass->methods, name, &method)) {
		RuntimeError(vm, "Undefined property '%s'", name->chars);
		return true;
	}

	return Call(vm, AS_CLOSURE(method), argCount);
}

bool GetMethod(VM* vm, Class* klass, String* name) {
	Value method;
	if (!GetTableEntry(&klass->methods, name, &method)) {
		RuntimeError(vm, "Undefined property '%s'.", name->chars);
		return false;
	}

	BoundMethod* boundMethod = NewBoundMethod(vm->garbageCollector, Peek(vm, 0), AS_CLOSURE(method));
	Pop(vm);
	Push(vm, OBJ_VAL(boundMethod));
	return true;
}

bool Invoke(VM* vm, String* name, int argCount) {
	Value receiver = Peek(vm, argCount);

	if (!IS_CLASS_INSTANCE(receiver)) {
		RuntimeError(vm, "Only instances have methods.");
		return false;
	}

	ClassInstance* instance = AS_CLASS_INSTANCE(receiver);

	Value value;
	if (GetTableEntry(&instance->fields, name, &value)) {
		vm->stackTop[-argCount - 1] = value;
		return CallValue(vm, value, argCount);
	}
	return InvokeFromClass(vm, instance->klass, name, argCount);
}