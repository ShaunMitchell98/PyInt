#include <stdlib.h>
#include <string.h>

// Include our platform compatibility header
#include "platform_compat.h"

#include "GarbageCollectionDisassembly.h"
#include "../Disassembly.h"
#include "../../../Types/Printing/Printing.h"
#include "../../../Types/Value/ValueFunctions.h"

#define BUFFER_SIZE 2000

static char* AllocateBuffer() {
    int bufferSize = BUFFER_SIZE * sizeof(char);
    char* buffer = (char*)malloc(bufferSize);
    *buffer = '\0';
    return buffer;
}

void WriteCollectionStart(IOSettings* outputSettings) {
    // Only output if the garbage collection output is enabled
    if (!outputSettings->enabled) {
        return;
    }
    
    char* buffer = AllocateBuffer();
    PYINT_STRCAT(buffer, BUFFER_SIZE, "Begin Garbage Collection\n");
    HandleOutput(buffer, outputSettings);
}

void WriteObjectAllocation(IOSettings* outputSettings, Object* object, int size) {
    // Only output if the garbage collection output is enabled
    if (!outputSettings->enabled) {
        return;
    }
    
    char* buffer = AllocateBuffer();
    PYINT_STRCAT(buffer, BUFFER_SIZE, "Allocate ");

    char text[10];
    PYINT_ITOA(size, text, 10, 10);
    PYINT_STRCAT(buffer, BUFFER_SIZE, text);
    
    PYINT_STRCAT(buffer, BUFFER_SIZE, " bytes\n");
    HandleOutput(buffer, outputSettings);
}

void WriteObjectMarking(IOSettings* outputSettings, Object* object) {
    // Only output if the garbage collection output is enabled
    if (!outputSettings->enabled) {
        return;
    }
    
    char* buffer = AllocateBuffer();
    PYINT_STRCAT(buffer, BUFFER_SIZE, "Mark ");
    PrintObject(outputSettings, OBJ_VAL(object), PROGRAM_OUTPUT, buffer, BUFFER_SIZE);
    PYINT_STRCAT(buffer, BUFFER_SIZE, "\n");
    HandleOutput(buffer, outputSettings);
}

void WriteObjectBlackened(IOSettings* outputSettings, Object* object) {
    // Only output if the garbage collection output is enabled
    if (!outputSettings->enabled) {
        return;
    }
    
    char* buffer = AllocateBuffer();
    PYINT_STRCAT(buffer, BUFFER_SIZE, "Blacken ");
    WriteValue(outputSettings, OBJ_VAL(object), PROGRAM_OUTPUT, buffer, BUFFER_SIZE);
    PYINT_STRCAT(buffer, BUFFER_SIZE, "\n");
    HandleOutput(buffer, outputSettings);
}

void WriteObjectDeallocation(IOSettings* outputSettings, Object* object) {
    // Only output if the garbage collection output is enabled
    if (!outputSettings->enabled) {
        return;
    }
    
    char* buffer = AllocateBuffer();
    PYINT_STRCAT(buffer, BUFFER_SIZE, "Free ");
    PrintObject(outputSettings, OBJ_VAL(object), GARBAGE_OUTPUT, buffer, BUFFER_SIZE);
    PYINT_STRCAT(buffer, BUFFER_SIZE, "\n");
    HandleOutput(buffer, outputSettings);
}

void WriteCollectionEnd(IOSettings* outputSettings) {
    // Only output if the garbage collection output is enabled
    if (!outputSettings->enabled) {
        return;
    }
    
    char* buffer = AllocateBuffer();
    PYINT_STRCAT(buffer, BUFFER_SIZE, "End Garbage Collection\n");
    HandleOutput(buffer, outputSettings);
}

void WriteCollectionAmount(IOSettings* outputSettings, size_t bytesBefore, size_t bytesAfter) {
    // Only output if the garbage collection output is enabled
    if (!outputSettings->enabled) {
        return;
    }
    
    char* buffer = AllocateBuffer();
    PYINT_STRCAT(buffer, BUFFER_SIZE, "Collected ");

    char text[10];
    PYINT_ITOA(bytesBefore - bytesAfter, text, 10, 10);
    PYINT_STRCAT(buffer, BUFFER_SIZE, text);
    PYINT_STRCAT(buffer, BUFFER_SIZE, " bytes\n");
    HandleOutput(buffer, outputSettings);
}
