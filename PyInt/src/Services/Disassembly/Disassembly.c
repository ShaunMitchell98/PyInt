#include <stdio.h>
#include <stdlib.h>

// Include our platform compatibility header
#include "platform_compat.h"

#include "Disassembly.h"

static void DisassembleToTerminal(char* buffer) {
    printf("%s", buffer);
}

static void DisassembleToFile(char* buffer, const char* filePath) {
    FILE file;
    FILE* fp = &file;
    
    #ifdef WIN32
    errno_t err = PYINT_FOPEN(fp, filePath, "a");
    if (err != 0) {
    #else
    PYINT_FOPEN(fp, filePath, "a");
    if (fp == NULL) {
    #endif
        fprintf(stderr, "Can't open bytecode disassembly output file: %s", filePath);
        return;
    }

    fputs(buffer, fp);
    fclose(fp);
}

void HandleOutput(char* buffer, IOSettings* settings) {
    if (settings->location == LOCATION_FILE && settings->filePathCount > 0) {
        // Use the first file path for output
        DisassembleToFile(buffer, settings->filePaths[0]);
        free(buffer);
    }
    else if (settings->location == LOCATION_TERMINAL) {
        DisassembleToTerminal(buffer);
        free(buffer);
    }
    else if (settings->location == LOCATION_STRING) {
        settings->string = buffer;
    }
}
