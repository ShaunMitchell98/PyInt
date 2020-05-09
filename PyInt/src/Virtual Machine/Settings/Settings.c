#include <string.h>
#include <stdlib.h>

#include "Settings.h"

static void InitialiseFile(IOSettings* settings, const char* filePath) {
    settings->enabled = true;
    settings->filePath = filePath;
    settings->location = LOCATION_FILE;
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

static bool Input(int i, const char* argv[], Settings* settings) {
    if (strcmp(argv[i], FILE_INPUT) == 0) {
        InitialiseFile(&settings->input, argv[i + 1]);
        settings->runMode = RUN_FILE;
    }
    else if (strcmp(argv[i], STRING_INPUT) == 0) {
        InitialiseString(&settings->input);
    }
    else {
        return false;
    }
    return true;
}

static bool Output(int i, const char* argv[], IOSettings* outputSettings) {
    if (strcmp(argv[i], FILE_OUTPUT) == 0) {
        InitialiseFile(outputSettings, argv[i + 1]);
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
        InitialiseFile(bytecodeSettings, argv[i + 1]);
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
        InitialiseFile(executionSettings, argv[i + 1]);
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
        InitialiseFile(garbageSettings, argv[i + 1]);
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
    settings->output.location = LOCATION_TERMINAL;
    settings->bytecode.enabled = false;
    settings->execution.enabled = false;
    settings->garbage.enabled = false;
    settings->runMode = RUN_REPL;

    for (int i = 0; i < argc; i++) {
        if (Input(i, argv, settings)) {
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
