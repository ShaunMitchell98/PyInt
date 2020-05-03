#ifndef PyInt_InterpreterSettings_h
#define PyInt_InterpreterSettings_h

#include "../../Headers/Common.h"
#include "../../Headers/RunMode.h"

#define STRING_SIZE 2000

#define TERMINAL_BYTECODE "-tb"
#define TERMINAL_EXECUTION "-te"
#define TERMINAL_OUTPUT "-to"

#define FILE_BYTECODE "-fb"
#define FILE_EXECUTION "-fe"
#define FILE_OUTPUT "-fo"
#define FILE_INPUT "-fi"

#define STRING_BYTECODE "-sb"
#define STRING_EXECUTION "-se"
#define STRING_OUTPUT "-so"
#define STRING_INPUT "-si"

typedef enum {
    LOCATION_TERMINAL,
    LOCATION_FILE,
    LOCATION_STRING
} Location;

typedef struct {
    Location location;
    const char* filePath;
    char* string;
    bool enabled;
} IOSettings;

typedef struct {
    RunMode runMode;
    IOSettings input;
    IOSettings output;
    IOSettings bytecode;
    IOSettings execution;
} Settings;

void InitialiseSettings(Settings* settings, int argc, const char* argv[]);

#endif
