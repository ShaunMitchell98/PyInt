#include <stdio.h>
#include <string.h>

#include "Errors.h"

void Error(const char* message) {
    fprintf(stderr, "%s", message);
}

void SyntaxError(IOSettings* outputSettings, Token* token) {
    if (outputSettings->location == LOCATION_FILE && outputSettings->filePathCount > 0) {
        // Use the first file path for error reporting
        fprintf(stderr, "File \\%s\\, line %d \n \t %s \n \t ^ \n SyntaxError: invalid syntax \n", 
            outputSettings->filePaths[0], token->line, token->start);
    }
    else {
        fprintf(stderr, "%s \n \t ^ \n SyntaxError: invalid syntax \n", token->start);
    }
}
