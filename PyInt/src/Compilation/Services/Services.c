#include "Services.h"

void InitParser(Parser* parser) {
	parser->hadError = false;
}

void InitServices(Services* services, Scanner* scanner, Parser* parser, IOSettings* bytecodeSettings, IOSettings* outputSettings, GarbageCollector* garbageCollector, Table* stringsTable) {
	services->scanner = scanner;
	services->parser = parser;
	services->bytecodeSettings = bytecodeSettings;
	services->outputSettings = outputSettings;
	services->garbageCollector = garbageCollector;
	services->stringsTable = stringsTable;
}