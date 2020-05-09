#ifndef PyInt_Value_h
#define PyInt_Value_h

#include "../../Headers/Common.h"
#include "../../Headers/PrintType.h"
#include "../Object/Object.h"
#include "../../Virtual Machine/Settings/Settings.h"

typedef enum {
	VAL_NUMBER,
    VAL_BOOL,
	VAL_CHAR,
    VAL_NONE,
    VAL_OBJ
} ValueType;

typedef struct {
	ValueType type;
	union {
		double number;
		char character;
        bool boolean;
        Object* obj;
	} as;
} Value;

typedef struct {
	int capacity;
	int count; 
	Value* values;
} ValueArray;

#define IS_NUMBER(value) ((value).type == VAL_NUMBER)
#define AS_NUMBER(value) ((value).as.number)
#define NUMBER_VAL(value) ((Value) { VAL_NUMBER, { .number = value}})

#define IS_NONE(value) ((value).type == VAL_NONE)
#define NONE_VAL ((Value) {VAL_NONE, { .number = 0}})

#define IS_BOOLEAN(value) ((value).type == VAL_BOOL)
#define AS_BOOLEAN(value) ((value).as.boolean)
#define BOOLEAN_VAL(value) ((Value) { VAL_BOOL, {.boolean = value}})

#define IS_CHAR(value) ((value).type == VAL_CHAR)
#define AS_CHAR(value) ((value).as.character)
#define CHAR_VAL(value) ((Value) {VAL_CHAR, {.character = value}})

#define IS_OBJ(value) ((value).type == VAL_OBJ)
#define AS_OBJ(value) ((value).as.obj)
#define OBJ_VAL(value) ((Value) { VAL_OBJ, {.obj = (Object*)value}})
#define OBJ_TYPE(value) (AS_OBJ(value)->type)

static inline bool IsObjType(Value value, ObjType type) {
	return IS_OBJ(value) && AS_OBJ(value)->type == type;
}

#endif

