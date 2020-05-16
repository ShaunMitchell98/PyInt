#include "ClassFunctions.h"
#include "../../Services/Memory/Memory.h"
#include "../Object/ObjectFunctions.h"

Class* NewClass(GarbageCollector* garbageCollector, String* name) {
	Class* klass = (Class*)AllocateObject(garbageCollector, sizeof(Class), CLASS);
	klass->name = name;
	return klass;
}