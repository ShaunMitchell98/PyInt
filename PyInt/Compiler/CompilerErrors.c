#include <stdio.h>
#include <string.h>

#include "Token.h"

#include "CompilerErrors.h"

void Error(const char* message) {
    fprintf(stderr, "%s", message);
}

void SyntaxError(Token* token) {
    fprintf(stderr, "File \\%s\\, line %d \n \t %s \n \t ^ \n SyntaxError: invalid syntax \n", token->tokenPath, token->line, token->start);
}
