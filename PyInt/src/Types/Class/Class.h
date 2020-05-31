#ifndef PyInt_Class_h
#define PyInt_Class_h

#include "../Object/Object.h"
#include "../String/String.h"
#include "../Value/Value.h"
#include "../../Services/Table/Table.h"
#include "../Closure/Closure.h"

#define INIT_ARRAY_SIZE 5

typedef struct {
	Object obj;
	String* name;
	Closure* init[INIT_ARRAY_SIZE];
	int initCount;
	Table methods;
} Class;

#define IS_CLASS(value) IsObjType(value, CLASS)
#define AS_CLASS(value) ((Class*)AS_OBJ(value))

#endif