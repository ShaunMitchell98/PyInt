#include <stdio.h>

#include "ScannerError.h"

void ScanError(const char* message) {
    fprintf(stderr, "%s\n", message);
}