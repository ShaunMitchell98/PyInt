#ifndef PyInt_ClassInstance_h
#define PyInt_ClasInstance_h

#include "../Object/Object.h"
#include "../Value/Value.h"
#include "../Class/Class.h"
#include "../../Services/Table/Table.h"

typedef struct {
	Object obj;
	Class* klass;
	Table fields;
} ClassInstance;

#define IS_CLASS_INSTANCE(value) IsObjType(value, CLASS_INSTANCE)
#define AS_CLASS_INSTANCE(value) ((ClassInstance*)AS_OBJ(value))

#endif