#include "ClassInstanceFunctions.h"
#include "../../Services/Memory/Memory.h"
#include "../Object/ObjectFunctions.h"
#include "../../Services/Table/TableFunctions.h"

ClassInstance* NewClassInstance(GarbageCollector* garbageCollector, Class* klass) {
	ClassInstance* instance = (ClassInstance*)AllocateObject(garbageCollector, sizeof(ClassInstance), CLASS_INSTANCE);
	instance->klass = klass;
	InitTable(&instance->fields);
	return instance;
}