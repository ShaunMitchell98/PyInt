#ifndef PyInt_InterpreterSettings_h
#define PyInt_InterpreterSettings_h

#include "../../Headers/Common.h"
#include "../../Headers/RunMode.h"

#define STRING_SIZE 2000

#define TERMINAL_BYTECODE "-tb"
#define TERMINAL_EXECUTION "-te"
#define TERMINAL_OUTPUT "-to"
#define TERMINAL_GARBAGE "-tg"

#define FILE_BYTECODE "-fb"
#define FILE_EXECUTION "-fe"
#define FILE_OUTPUT "-fo"
#define FILE_INPUT "-fi"
#define FILE_GARBAGE "-fg"

#define STRING_BYTECODE "-sb"
#define STRING_EXECUTION "-se"
#define STRING_OUTPUT "-so"
#define STRING_INPUT "-si"
#define STRING_GARBAGE "-sg"

// Maximum number of input files
#define MAX_INPUT_FILES 100

typedef enum {
    LOCATION_TERMINAL,
    LOCATION_FILE,
    LOCATION_STRING
} Location;

typedef struct {
    Location location;
    const char** filePaths;  // Array of file paths
    int filePathCount;       // Number of file paths
    char* string;
    bool enabled;
} IOSettings;

typedef struct {
    RunMode runMode;
    IOSettings input;
    IOSettings output;
    IOSettings bytecode;
    IOSettings execution;
    IOSettings garbage;
} Settings;

void InitialiseSettings(Settings* settings, int argc, const char* argv[]);

#endif
