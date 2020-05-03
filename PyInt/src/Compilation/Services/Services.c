#include "Services.h"

void InitParser(Parser* parser) {
	parser->hadError = false;
}

void InitServices(Services* services, Scanner* scanner, Parser* parser, IOSettings* bytecodeSettings, IOSettings* outputSettings, Object* heap, Table* stringsTable) {
	services->scanner = scanner;
	services->parser = parser;
	services->bytecodeSettings = bytecodeSettings;
	services->outputSettings = outputSettings;
	services->heap = heap;
	services->stringsTable = stringsTable;
}