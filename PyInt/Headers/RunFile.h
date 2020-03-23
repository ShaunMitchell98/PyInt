#ifndef PyInt_RunFile_h
#define PyInt_RunFile_h

#include "VM.h"
char* ReadFile(const char* path);
void RunFile(const char* path, PrintInfo printInfo);

#endif
