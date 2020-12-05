#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "FunctionalTestUtilities.h"
#include "../../PyInt/src/Virtual Machine/Settings/Settings.h"
#include "../../PyInt/src/RunFile.h"
#include "../../PyInt/src/Virtual Machine/VM/VM.h"
#include "../../PyInt/src/Virtual Machine/VM/VMFunctions.h"

static Settings InitialiseTestSettings() {
	Settings settings;
	settings.output.location = LOCATION_STRING;
	settings.bytecode.enabled = false;
	settings.execution.enabled = false;
	settings.garbage.enabled = false;
	settings.bytecode.filePath = "C:\\Users\\User\\Documents\\Bytecode.txt";
	settings.bytecode.location = LOCATION_FILE;
	settings.execution.filePath = "C:\\Users\\User\\Documents\\Execution.txt";
	settings.execution.location = LOCATION_FILE;
	settings.garbage.filePath = "C:\\Users\\User\\Documents\\GarbageCollection.txt";
	settings.garbage.location = LOCATION_FILE;
	settings.output.string = (char*)malloc(100);
	settings.runMode = RUN_FILE;
	settings.output.string[0] = '\0';
	return settings;
}

char* RunInterpreter(char* code) {
	Settings settings = InitialiseTestSettings();
	Interpret(code, &settings);
	return settings.output.string;
}

char* RunInterpreterFromFile(char* path) {
	char* fullPath = (char*)malloc(150);
	fullPath[0] = '\0';
	strcat_s(fullPath, 150, "C:\\Users\\Shaun Mitchell\\source\\repos\\PyInt\\Tests\\Test Programs\\");
	strcat_s(fullPath, 150, path);
	strcat_s(fullPath, 150, ".txt");
	char* code = ReadFile(fullPath);
	free(fullPath);
	return RunInterpreter(code);
}