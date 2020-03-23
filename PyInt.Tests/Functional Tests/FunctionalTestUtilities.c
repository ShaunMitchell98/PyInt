#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "RunFile.h"

#include "FunctionalTestUtilities.h"
#include "PrintInfo.h"

static char* GetOutputFilePath(char fileName[]) {
	char outputDirectory[] = "C://Users//User//source//repos//PyInt//PyInt.Tests//Functional Tests//Output//";
	char fileExtension[] = ".txt";
	int bufferSize = strlen(outputDirectory) + strlen(fileName) + strlen(fileExtension) + 1;
	char* buffer = (char*)malloc(bufferSize);

    strcpy_s(buffer, bufferSize, outputDirectory);
	strcat_s(buffer, bufferSize, fileName);
	strcat_s(buffer, bufferSize, fileExtension);
	return buffer;
}

static char* GetSourceFilePath(char fileName[]) {
	char sourceDirectory[] = "C://Users//User//source//repos//PyInt//PyInt.Tests//Functional Tests//Source Code//";
	char fileExtension[] = ".txt";
	int bufferSize = strlen(sourceDirectory) + strlen(fileName) + strlen(fileExtension) + 1;
	char* buffer = (char*)malloc(bufferSize * sizeof(char));
	
	strcpy_s(buffer, bufferSize, sourceDirectory);
	strcat_s(buffer, bufferSize, fileName);
	strcat_s(buffer, bufferSize, fileExtension);
	return buffer;
}

static PrintInfo GetPrintInfo(char* fileName) {
	PrintInfo printInfo;
	printInfo.printLocation = PRINT_FILE;
	printInfo.filePath = GetOutputFilePath(fileName);
	return printInfo;
}

char* RunInterpreter(char* fileName) {
	PrintInfo printInfo = GetPrintInfo(fileName);
	const char* filePath = GetSourceFilePath(fileName);
	RunFile(filePath, printInfo);
	return ReadFile(printInfo.filePath);
}

void ClearOutputFile(char* fileName) {
	char* filePath = GetOutputFilePath(fileName);
	remove(filePath);
}