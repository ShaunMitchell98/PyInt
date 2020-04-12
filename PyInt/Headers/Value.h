#ifndef PyInt_Value_h
#define PyInt_Value_h

#include "Common.h"
#include "PrintType.h"
#include "InterpreterSettings.h"

typedef struct sObj Obj;
typedef struct sObjString ObjString;
typedef struct sObjFunction ObjFunction;

typedef enum {
	VAL_NUMBER,
    VAL_STRING,
    VAL_CHAR,
    VAL_BOOL,
    VAL_NONE,
    VAL_OBJ
} ValueType;

typedef struct {
	ValueType type;
	union {
		double number;
        bool boolean;
        char character;
        Obj* obj;
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
#define CHAR_VAL(value) ((Value) { VAL_CHAR, { .character = value}})

#define IS_OBJ(value) ((value).type == VAL_OBJ)
#define AS_OBJ(value) ((value).as.obj)
#define OBJ_VAL(value) ((Value) { VAL_OBJ, {.obj = (Obj*)value}})

void InitValueArray(ValueArray* array);
void WriteValueArray(ValueArray* array, Value value);
void FreeValueArray(ValueArray* array);
void WriteValue(IOSettings* settings, Value value, PrintType printType, char* buffer, int bufferSize);

#endif

