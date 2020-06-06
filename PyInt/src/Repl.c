#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Virtual Machine/VM/VM.h"
#include "Repl.h"

void Repl(Settings* settings) {
    VM vm;
    GarbageCollector garbageCollector;
    InitVM(&vm, &garbageCollector, settings);
    
    char line[1024];
    while (true) {
        printf("\n");
        printf(">>> ");

        if (!fgets(line, sizeof(line), stdin)) {
            printf("\n");
            break;
        }

        if (strcmp(line, "quit\n") == 0) {
            printf("Use quit() or Ctrl-D (i.e. EOF) to exit");
            Repl(settings);
            return;
        }
        else if (strcmp(line, "quit()\n") == 0 || strcmp(line, "Ctrl-D") == 0) {
            FreeVM(&vm);
            exit(0);
        }

        Interpret(&vm, line, settings);
    }
}