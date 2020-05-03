#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "RunFile.h"
#include "Virtual Machine/VM/VM.h"

char* ReadFile(const char* path) {
    FILE file;
    FILE* fp = &file;
    errno_t err = fopen_s(&fp, path, "rb");
    if (err != 0) {
        fprintf(stderr, "Python: can't open file '%s' [Errno 2] No such file or directory\n",
            path);
        exit(74);
    }

    fseek(fp, 0L, SEEK_END);
    size_t fileSize = ftell(fp);
    rewind(fp);

    char* buffer = (char*)malloc(fileSize + 1);
    if (buffer == NULL) {
        fprintf(stderr, "Not enough memory to read \"%s\".\n", path);
        exit(74);
    }
    size_t bytesRead = fread(buffer, sizeof(char), fileSize, fp);
    if (bytesRead < fileSize) {
        fprintf(stderr, "Could not read file \"%s\".\n", path);
        exit(74);
    }

    buffer[bytesRead] = '\0';

    fclose(fp);
    return buffer;
}

void RunFile(Settings* settings) {
    char* sourceCode = ReadFile(settings->input.filePath);

    InterpretResult result = Interpret(sourceCode, settings);
    free(sourceCode);

    if (result == INTERPRET_COMPILE_ERROR) exit(65);
    if (result == INTERPRET_RUNTIME_ERROR) exit(70);
}