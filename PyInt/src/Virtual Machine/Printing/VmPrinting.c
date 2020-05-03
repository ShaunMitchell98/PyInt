#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "VmPrinting.h"
#include "../../Types/Printing/Printing.h"

static bool WriteToFile(const char* text, const char* filePath) {
    FILE file;
    FILE* fp = &file;
    errno_t err = fopen_s(&fp, filePath, "wb");

    if (err != 0) {
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
        printf(buffer);
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
        _itoa_s(AS_NUMBER(a), text, 10, 10);
    }
    else if (IS_BOOLEAN(a)) {
        AS_BOOLEAN(a) == true ? strcpy_s(text, 10, "true") : strcpy_s(text, 10, "false");
    }
    else if (IS_CHAR(a)) {
        strcpy_s(text, 10, &AS_CHAR(a));
    }
    else if (IS_OBJ(a)) {
        PrintObject(settings, a, PROGRAM_OUTPUT, text, 10);
    }
    else {
        return false;
    }
    WriteToFile(text, settings->filePath);
    return true;
}

bool PrintValueToString(IOSettings* settings, Value a) {
    if (IS_NUMBER(a)) {
        char numOutput[10] = "";
        _itoa_s(AS_NUMBER(a), numOutput, 10, 10);
        strcat_s(settings->string, 100, numOutput);
    }
    else if (IS_BOOLEAN(a)) {
        AS_BOOLEAN(a) == true ? strcat_s(settings->string, 100, "true") : strcat_s(settings->string, 100, "false");
    }
    else if (IS_CHAR(a)) {
        strcat_s(settings->string, 100, &AS_CHAR(a));
    }
    else if (IS_OBJ(a)) {
        PrintObject(settings, a, TEST_OUTPUT, settings->string, 100);
    }
    else {
        return false;
    }
    return true;
}