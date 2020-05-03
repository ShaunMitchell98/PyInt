#include <stdio.h>
#include <stdlib.h>

#include "Disassembly.h"

static void DisassembleToTerminal(char* buffer) {
    printf(buffer);
}

static void DisassembleToFile(char* buffer, const char* filePath) {
    FILE file;
    FILE* fp = &file;
    errno_t err = fopen_s(&fp, filePath, "a");

    if (err != 0) {
        fprintf(stderr, "Can't open bytecode disassembly output file: %s", filePath);
        return;
    }

    fputs(buffer, fp);
    fclose(fp);
}

void HandleOutput(char* buffer, IOSettings* settings) {
    if (settings->location == LOCATION_FILE) {
        DisassembleToFile(buffer, settings->filePath);
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
