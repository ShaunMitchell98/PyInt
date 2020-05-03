#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "FunctionalTestUtilities.h"
#include "../PyInt/src/Virtual Machine/Settings/Settings.h"
#include "../PyInt/src/RunFile.h"
#include "../PyInt/src/Virtual Machine/VM/VM.h"

static Settings InitialiseTestSettings() {
	Settings settings;
	settings.output.location = LOCATION_STRING;
	settings.bytecode.enabled = true;
	settings.execution.enabled = true;
	settings.bytecode.filePath = "C:\\Users\\User\\Documents\\Bytecode.txt";
	settings.bytecode.location = LOCATION_FILE;
	settings.execution.filePath = "C:\\Users\\User\\Documents\\Execution.txt";
	settings.execution.location = LOCATION_FILE;
	settings.output.string = (char*)malloc(100);
	settings.output.string[0] = '\0';
	return settings;
}

char* RunInterpreter(char* code) {
	Settings settings = InitialiseTestSettings();
	Interpret(code, &settings);
	return settings.output.string;
}