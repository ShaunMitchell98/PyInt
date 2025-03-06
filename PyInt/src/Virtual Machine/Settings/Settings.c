#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "Settings.h"

static void InitialiseFile(IOSettings* settings, const char* filePath) {
    settings->enabled = true;
    settings->location = LOCATION_FILE;
    
    // If this is the first file, initialize the array
    if (settings->filePaths == NULL) {
        settings->filePaths = (const char**)malloc(sizeof(const char*));
        settings->filePathCount = 0;
    } else {
        // Resize the array to accommodate the new file path
        settings->filePaths = (const char**)realloc(settings->filePaths, 
                                         (settings->filePathCount + 1) * sizeof(const char*));
    }
    
    // Add the new file path to the array
    settings->filePaths[settings->filePathCount] = filePath;
    settings->filePathCount++;
}

static void InitialiseString(IOSettings* settings) {
    settings->enabled = true;
    settings->string = (char*)malloc(STRING_SIZE);
    settings->location = LOCATION_STRING;
}

static void InitialiseTerminal(IOSettings* settings) {
    settings->enabled = true;
    settings->location = LOCATION_TERMINAL;
}

static bool Input(int i, const char* argv[], int argc, Settings* settings) {
    if (strcmp(argv[i], FILE_INPUT) == 0) {
        // Process multiple input files
        // We need at least one file path after the -fi flag
        if (i + 1 >= argc) {
            fprintf(stderr, "Error: -fi flag requires at least one file path\n");
            exit(64);
        }

        // Initialize file path collection if needed
        if (settings->input.filePaths == NULL) {
            settings->input.filePaths = (const char**)malloc(MAX_INPUT_FILES * sizeof(const char*));
            settings->input.filePathCount = 0;
        }

        // Get file paths until we reach another flag or the end of arguments
        int j = i + 1;
        while (j < argc && argv[j][0] != '-') {
            if (settings->input.filePathCount >= MAX_INPUT_FILES) {
                fprintf(stderr, "Error: Too many input files. Maximum is %d\n", MAX_INPUT_FILES);
                exit(64);
            }
            
            settings->input.filePaths[settings->input.filePathCount++] = argv[j];
            j++;
        }
        
        // Set the run mode to file
        settings->runMode = RUN_FILE;
        settings->input.location = LOCATION_FILE;
        settings->input.enabled = true;
        
        // Return the number of arguments consumed
        return true;
    }
    else if (strcmp(argv[i], STRING_INPUT) == 0) {
        InitialiseString(&settings->input);
    }
    else {
        return false;
    }
    return true;
}

// Special InitialiseFile function for non-input settings (output, bytecode, etc)
// These still use a single file path
static void InitialiseSingleFile(IOSettings* settings, const char* filePath) {
    settings->enabled = true;
    settings->location = LOCATION_FILE;
    
    // Initialize with a single file path
    if (settings->filePaths == NULL) {
        settings->filePaths = (const char**)malloc(sizeof(const char*));
        settings->filePathCount = 0;
    }
    
    // Set the single file path
    settings->filePaths[0] = filePath;
    settings->filePathCount = 1;
}

static bool Output(int i, const char* argv[], IOSettings* outputSettings) {
    if (strcmp(argv[i], FILE_OUTPUT) == 0) {
        InitialiseSingleFile(outputSettings, argv[i + 1]);
    }
    else if (strcmp(argv[i], STRING_OUTPUT) == 0) {
        InitialiseString(outputSettings);
    }
    else if (strcmp(argv[i], TERMINAL_OUTPUT) == 0) {
        InitialiseTerminal(outputSettings);
    }
    else {
        return false;
    }
    return true;
}

static bool Bytecode(int i, const char* argv[], IOSettings* bytecodeSettings) {
    if (strcmp(argv[i], FILE_BYTECODE) == 0) {
        InitialiseSingleFile(bytecodeSettings, argv[i + 1]);
    }
    else if (strcmp(argv[i], STRING_BYTECODE) == 0) {
        InitialiseString(bytecodeSettings);
    }
    else if (strcmp(argv[i], TERMINAL_BYTECODE) == 0) {
        InitialiseTerminal(bytecodeSettings);
    }
    else {
        return false;
    }
    return true;
}

static bool Execution(int i, const char* argv[], IOSettings* executionSettings) {
    if (strcmp(argv[i], FILE_EXECUTION) == 0) {
        InitialiseSingleFile(executionSettings, argv[i + 1]);
    }
    else if (strcmp(argv[i], STRING_EXECUTION) == 0) {
        InitialiseString(executionSettings);
    }
    else if (strcmp(argv[i], TERMINAL_EXECUTION) == 0) {
        InitialiseTerminal(executionSettings);
    }
    else {
        return false;
    }
    return true;
}

static bool Garbage(int i, const char* argv[], IOSettings* garbageSettings) {
    if (strcmp(argv[i], FILE_GARBAGE) == 0) {
        InitialiseSingleFile(garbageSettings, argv[i + 1]);
    }
    else if (strcmp(argv[i], STRING_GARBAGE) == 0) {
        InitialiseString(garbageSettings);
    }
    else if (strcmp(argv[i], TERMINAL_GARBAGE) == 0) {
        InitialiseTerminal(garbageSettings);
    }
    else {
        return false;
    }
    return true;
}

void InitialiseSettings(Settings* settings, int argc, const char* argv[]) {
    settings->input.location = LOCATION_TERMINAL;
    settings->input.filePaths = NULL;
    settings->input.filePathCount = 0;
    settings->output.location = LOCATION_TERMINAL;
    settings->output.filePaths = NULL;
    settings->output.filePathCount = 0;
    settings->bytecode.enabled = false;
    settings->bytecode.filePaths = NULL;
    settings->bytecode.filePathCount = 0;
    settings->execution.enabled = false;
    settings->execution.filePaths = NULL;
    settings->execution.filePathCount = 0;
    settings->garbage.enabled = false;
    settings->garbage.filePaths = NULL;
    settings->garbage.filePathCount = 0;
    settings->runMode = RUN_REPL;

    for (int i = 0; i < argc; i++) {
        if (Input(i, argv, argc, settings)) {
            // Skip the flag and all the file paths we've processed
            while (i + 1 < argc && argv[i + 1][0] != '-') {
                i++;
            }
            continue;
        }
        else if (Output(i, argv, &settings->output)) {
            continue;
        }
        else if (Bytecode(i, argv, &settings->bytecode)) {
            continue;
        }
        else if (Execution(i, argv, &settings->execution)) {
            continue;
        }
        else if (Garbage(i, argv, &settings->garbage)) {
            continue;
        }
    }
}
