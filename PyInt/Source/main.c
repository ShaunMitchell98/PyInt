#include <stdio.h>
#include <stdlib.h>

#include "../Headers/InterpreterSettings.h"
#include "../Headers/Repl.h"
#include "../Headers/RunFile.h"

int main(int argc, const char* argv[]) {
    
    InterpreterSettings settings;
    InitialiseSettings(&settings, argc, argv);

    if (settings.runMode == RUN_REPL) {
        Repl(settings);
    }
    else if (settings.runMode == RUN_FILE) {
        RunFile(settings);
    }
    else {
        fprintf(stderr, "No run mode specified");
        return -1;
    }
    return 0;
}
