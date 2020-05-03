#ifndef PyInt_RunFile_h
#define PyInt_RunFile_h

#include "Virtual Machine/Settings/Settings.h"

char* ReadFile(const char* path);
void RunFile(Settings* settings);

#endif
