#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../Headers/Common.h"
#include "../Headers/Bytecode.h"
#include "../Headers/Debug.h"
#include "../Headers/VM.h"

static void Repl() {
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
            Repl();
            return;
        }
        else if (strcmp(line, "quit()\n") == 0 || strcmp(line, "Ctrl-D") == 0) {
            exit(0);
        }
        
        Interpret(line, "<stdin>");
    }
}

static char* ReadFile(const char* path) {
    FILE file;
    FILE* fp = &file;
    errno_t err= fopen_s(&fp, path, "rb");
    if (err != 0) {
        fprintf(stderr, "Python: can't open file '%s' [Errno 2] No such file or directory\n",
                path);
        exit(74);
    }
    
    fseek(fp, 0L, SEEK_END);
    size_t fileSize = ftell(fp);
    rewind(fp);
    
    char* buffer = (char*)malloc(fileSize+1);
    if (buffer == NULL) {
        fprintf(stderr, "Not enough memory to read \"%s\".\n", path);
        exit(74);
    }
    size_t bytesRead = fread(buffer, sizeof(char), fileSize, fp);
    if (bytesRead < fileSize) {
        fprintf(stderr, "Could not read file \"%s\".\n", path);
        exit(74);
    }
    
    buffer[bytesRead] = '\0';
    
    fclose(fp);
    return buffer;
}

static void RunFile(const char* path) {
    char* sourceCode = ReadFile(path);
    InterpretResult result = Interpret(sourceCode, path);
    free(sourceCode);
    
    if (result == INTERPRET_COMPILE_ERROR) exit(65);
    if (result == INTERPRET_RUNTIME_ERROR) exit(70);
}

int main(int argc, const char* argv[]) {
    
    //if (argc == 0) {
      //  Repl();
   // }
 //   else if (argc == 1) {
  //      RunFile(argv[1]);
  //  }
  //  else {
    //    fprintf(stderr, "Python: can't open file %s: [Errno 2] No such file or directory",
     //           argv[1]);
      //  return -1;
    RunFile("C:\\Users\\User\\source\\repos\\PyInt\\PyInt\\PyInt_Script");
   // }
	return 0;
}
