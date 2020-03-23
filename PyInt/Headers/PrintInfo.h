#ifndef PyInt_PrintInfo_h
#define PyInt_PrintInfo_h

typedef enum {
    PRINT_TERMINAL,
    PRINT_FILE
} PrintLocation;

typedef struct {
    const char* filePath;
} PrintFile;

typedef struct {
    PrintLocation printLocation;
    const char* filePath;
} PrintInfo;

#endif
