#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../Headers/Bytecode.h"
#include "../Headers/Common.h"
#include "../Headers/Debug.h"
#include "../Headers/Repl.h"
#include "../Headers/RunFile.h"
#include "../Headers/VM.h"

int main(int argc, const char* argv[]) {
    
    if (argc == 1) {
        Repl(PRINT_TERMINAL);
    }
    else if (argc == 2) {
        PrintInfo printInfo;
        printInfo.printLocation = PRINT_TERMINAL;
        RunFile(argv[1], printInfo);
    }
    else {
        for (int i = 0; i < argc; i++) {
            if (argv[i] == "-o") {
                PrintInfo printInfo;
                printInfo.printLocation = PRINT_FILE;
                printInfo.filePath = argv[i + 1];
                RunFile(argv[1], printInfo);
                return 0;
            }
        }
        fprintf(stderr, "Python: can't open file %s: [Errno 2] No such file or directory",
            argv[1]);
        return -1;
    }
}
