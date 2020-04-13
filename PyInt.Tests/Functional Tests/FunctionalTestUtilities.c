#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "RunFile.h"

#include "FunctionalTestUtilities.h"
#include "InterpreterSettings.h"

static InterpreterSettings InitialiseTestSettings() {
	InterpreterSettings settings;
	settings.output.location = LOCATION_STRING;
	settings.bytecode.enabled = false;
	settings.execution.enabled = false;
	settings.output.string = (char*)malloc(100);
	settings.execution.enabled = true;
	settings.execution.location = LOCATION_FILE;
	settings.execution.filePath = "C:\\Users\\User\\Documents\\Execution.txt";
	settings.output.string[0] = '\0';
	return settings;
}

char* RunInterpreter(char* code) {
	InterpreterSettings settings = InitialiseTestSettings();
	Interpret(code, settings);
	return settings.output.string;
}