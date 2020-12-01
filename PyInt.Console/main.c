#include <stdio.h>
#include <stdlib.h>

#include "Virtual Machine/Settings/Settings.h"
#include "Repl.h"
#include "RunFile.h"

int main(int argc, const char* argv[]) {

    Settings settings;
    InitialiseSettings(&settings, argc, argv);

    if (settings.runMode == RUN_REPL) {
        Repl(&settings);
    }
    else if (settings.runMode == RUN_FILE) {
        RunFile(&settings);
    }
    else {
        fprintf(stderr, "No run mode specified");
        return -1;
    }
    return 0;
}