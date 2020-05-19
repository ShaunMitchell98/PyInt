#ifndef PyInt_BoundMethod_h
#define PyInt_BoundMethod_h

#include "../Object/Object.h"
#include "../Value/Value.h"
#include "../Closure/Closure.h"

typedef struct {
	Object obj;
	Value receiver;
	Closure* method;
} BoundMethod;

#define IS_BOUND_METHOD(value) IsObjType(value, BOUND_METHOD)
#define AS_BOUND_METHOD(value) ((BoundMethod*)AS_OBJ(value))

#endif