#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "RunFile.h"

#include "FunctionalTestUtilities.h"
#include "PrintInfo.h"

static PrintInfo GetPrintInfo() {
	PrintInfo printInfo;
	printInfo.printLocation = PRINT_STRING;
	printInfo.output = (char*)malloc(100);
	printInfo.output[0] = '\0';
	return printInfo;
}

char* RunInterpreter(char* code) {
	PrintInfo printInfo = GetPrintInfo();
	Interpret(code, "string", printInfo);
	return printInfo.output;
}