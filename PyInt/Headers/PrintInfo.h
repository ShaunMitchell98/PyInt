#ifndef PyInt_PrintInfo_h
#define PyInt_PrintInfo_h

typedef enum {
    PRINT_TERMINAL,
    PRINT_FILE,
    PRINT_STRING
} PrintLocation;

typedef struct {
    PrintLocation printLocation;
    const char* filePath;
    char* output;
} PrintInfo;

#endif
