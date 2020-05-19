#include "BoundMethodFunctions.h"
#include "../Object/ObjectFunctions.h"

BoundMethod* NewBoundMethod(GarbageCollector* garbageCollector, Value receiver, Closure* method) {
	BoundMethod* boundMethod = (BoundMethod*)AllocateObject(garbageCollector, sizeof(BoundMethod), BOUND_METHOD);
	boundMethod->receiver = receiver;
	boundMethod->method = method;
	return boundMethod;
}