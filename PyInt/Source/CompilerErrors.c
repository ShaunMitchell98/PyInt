#include <stdio.h>
#include <string.h>

#include "../Headers/Token.h"
#include "../Headers/CompilerErrors.h"
#include "../Headers/VM.h"

void Error(const char* message) {
    fprintf(stderr, "%s", message);
}

void SyntaxError(VM* vm, Token* token) {

    if (vm->settings.output.location == LOCATION_FILE) {
        fprintf(stderr, "File \\%s\\, line %d \n \t %s \n \t ^ \n SyntaxError: invalid syntax \n", vm->settings.output.filePath, token->line, token->start);
    }
    else {
        fprintf(stderr, "%s \n \t ^ \n SyntaxError: invalid syntax \n", token->start);

    }
}
