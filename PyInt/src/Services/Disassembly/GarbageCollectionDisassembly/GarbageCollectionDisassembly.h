#ifndef PyInt_GarbageCollectionDisassembly_h
#define PyInt_GarbageCollectionDisassembly_h

#include "../../../Virtual Machine/Settings/Settings.h"
#include "../../../Types/Object/Object.h"

void WriteCollectionStart(IOSettings* outputSettings);
void WriteObjectAllocation(IOSettings* outputSettings, Object* object, int size);
void WriteObjectDeallocation(IOSettings* outputSettings, Object* object);
void WriteObjectMarking(IOSettings* outputSettings, Object* object);
void WriteObjectBlackened(IOSettings* outputSettings, Object* object);
void WriteCollectionEnd(IOSettings* outputSettings);
void WriteCollectionAmount(IOSettings* outputSettings, size_t bytesBefore, size_t bytesAfter);

#endif