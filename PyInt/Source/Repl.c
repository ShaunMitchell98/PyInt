#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../Headers/Repl.h"
#include "../Headers/Common.h"
#include "../Headers/VM.h"

void Repl(PrintLocation printLocation) {
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
            Repl(printLocation);
            return;
        }
        else if (strcmp(line, "quit()\n") == 0 || strcmp(line, "Ctrl-D") == 0) {
            exit(0);
        }

        char* filePath = "Output.txt";
        PrintInfo printInfo;
        printInfo.filePath = filePath;

        Interpret(line, "<stdin>", printInfo);
    }
}