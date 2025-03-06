#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Include our platform compatibility header
#include "platform_compat.h"

#include "RunFile.h"
#include "Virtual Machine/VM/VMFunctions.h"

char* ReadFile(const char* path) {
    FILE file;
    FILE* fp = &file;
    
    #ifdef WIN32
    errno_t err = PYINT_FOPEN(fp, path, "rb");
    if (err != 0) {
    #else
    PYINT_FOPEN(fp, path, "rb");
    if (fp == NULL) {
    #endif
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

// Concatenate multiple files into a single source code string with appropriate newlines
char* ConcatenateSourceCode(const char** filePaths, int fileCount) {
    // First, calculate the total size needed
    size_t totalSize = 0;
    char** fileContents = (char**)malloc(fileCount * sizeof(char*));
    size_t* fileSizes = (size_t*)malloc(fileCount * sizeof(size_t));
    
    for (int i = 0; i < fileCount; i++) {
        fileContents[i] = ReadFile(filePaths[i]);
        fileSizes[i] = strlen(fileContents[i]);
        totalSize += fileSizes[i];
        // Add space for a newline after each file (except potentially the last one)
        totalSize += 1;
    }
    
    // Allocate the combined buffer (+1 for null terminator)
    char* combinedSource = (char*)malloc(totalSize + 1);
    if (combinedSource == NULL) {
        fprintf(stderr, "Not enough memory to combine source files.\n");
        exit(74);
    }
    
    // Copy all file contents into the combined buffer with newlines between them
    size_t offset = 0;
    for (int i = 0; i < fileCount; i++) {
        memcpy(combinedSource + offset, fileContents[i], fileSizes[i]);
        offset += fileSizes[i];
        
        // Add a newline after each file (including the last one for clean parsing)
        combinedSource[offset] = '\n';
        offset++;
        
        // Free the individual file content
        free(fileContents[i]);
    }
    
    // Null terminate the combined string
    combinedSource[offset] = '\0';
    
    // Free temporary arrays
    free(fileContents);
    free(fileSizes);
    
    return combinedSource;
}

void RunFile(Settings* settings) {
    char* sourceCode;
    
    if (settings->input.filePathCount == 1) {
        // If there's only one file, just read it directly
        sourceCode = ReadFile(settings->input.filePaths[0]);
    } else {
        // If there are multiple files, concatenate them
        sourceCode = ConcatenateSourceCode(settings->input.filePaths, settings->input.filePathCount);
    }

    InterpretResult result = Interpret(sourceCode, settings);
    free(sourceCode);

    if (result == INTERPRET_COMPILE_ERROR) exit(65);
    if (result == INTERPRET_RUNTIME_ERROR) exit(70);
}