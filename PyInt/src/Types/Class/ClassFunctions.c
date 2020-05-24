#include "ClassFunctions.h"
#include "../../Services/Memory/Memory.h"
#include "../Object/ObjectFunctions.h"
#include "../../Services/Table/TableFunctions.h"
#include "../../Services/Bytecode/BytecodeFunctions.h"

Class* NewClass(GarbageCollector* garbageCollector, String* name) {
	Class* klass = (Class*)AllocateObject(garbageCollector, sizeof(Class), CLASS);
	klass->name = name;
	InitTable(&klass->methods);
	klass->init = NULL;
	return klass;
}