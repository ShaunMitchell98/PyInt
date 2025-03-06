#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Include our platform compatibility header
#include "platform_compat.h"

#include "VmPrinting.h"
#include "../../Types/Printing/Printing.h"

static bool WriteToFile(const char* text, const char* filePath) {
    FILE file;
    FILE* fp = &file;
    
    #ifdef WIN32
    errno_t err = PYINT_FOPEN(fp, filePath, "wb");
    if (err != 0) {
    #else
    PYINT_FOPEN(fp, filePath, "wb");
    if (fp == NULL) {
    #endif
        fprintf(stderr, "Cannot write to file %s", filePath);
        return false;
    }

    fputs(text, fp);
    fclose(fp);
    return true;
}

bool PrintValueToTerminal(IOSettings* settings, Value a) {
    if (IS_NUMBER(a)) {
        printf("%f\n", AS_NUMBER(a));
    }
    else if (IS_BOOLEAN(a)) {
        printf(AS_BOOLEAN(a) ? "True\n" : "False\n");
    }
    else if (IS_CHAR(a)) {
        printf("%c", AS_CHAR(a));
    }
    else if (IS_OBJ(a)) {
        char* buffer = (char*)malloc(100 * sizeof(char));
        PrintObject(settings, a, PROGRAM_OUTPUT, buffer, 100);
        printf("%s", buffer);
        free(buffer);
    }
    else {
        return false;
    }
    return true;
}

bool PrintValueToFile(IOSettings* settings, Value a) {
    char text[10];
    if (IS_NUMBER(a)) {
        PYINT_ITOA((int)AS_NUMBER(a), text, 10, 10);
    }
    else if (IS_BOOLEAN(a)) {
        AS_BOOLEAN(a) == true ? PYINT_STRCPY(text, 10, "true") : PYINT_STRCPY(text, 10, "false");
    }
    else if (IS_CHAR(a)) {
        PYINT_STRCPY(text, 10, &AS_CHAR(a));
    }
    else if (IS_OBJ(a)) {
        PrintObject(settings, a, PROGRAM_OUTPUT, text, 10);
    }
    else {
        return false;
    }
    
    // Use the first file path for output if available
    if (settings->filePathCount > 0) {
        WriteToFile(text, settings->filePaths[0]);
    }
    return true;
}

bool PrintValueToString(IOSettings* settings, Value a) {
    if (IS_NUMBER(a)) {
        char numOutput[10] = "";
        PYINT_ITOA((int)AS_NUMBER(a), numOutput, 10, 10);
        PYINT_STRCAT(settings->string, 100, numOutput);
    }
    else if (IS_BOOLEAN(a)) {
        AS_BOOLEAN(a) == true ? PYINT_STRCAT(settings->string, 100, "true") : PYINT_STRCAT(settings->string, 100, "false");
    }
    else if (IS_CHAR(a)) {
        PYINT_STRCAT(settings->string, 100, &AS_CHAR(a));
    }
    else if (IS_OBJ(a)) {
        PrintObject(settings, a, TEST_OUTPUT, settings->string, 100);
    }
    else {
        return false;
    }
    return true;
}