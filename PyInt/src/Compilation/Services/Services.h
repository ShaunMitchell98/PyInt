#ifndef PyInt_Services_h
#define PyInt_Services_h

#include "../../Headers/Common.h"
#include "../../Services/Scanner/Scanner.h"
#include "../../Virtual Machine/Settings/Settings.h"
#include "../../Services/Table/Table.h"
#include "../../Types/Object/Object.h"

typedef struct {
    Token previous;
    Token current;
    bool hadError;
} Parser;

typedef struct {
    Parser* parser;
    Scanner* scanner;
    IOSettings* bytecodeSettings;
    IOSettings* outputSettings;
    Object* heap;
    Table* stringsTable;
} Services;

void InitParser(Parser* parser);
void InitServices(Services* services, Scanner* scanner, Parser* parser, IOSettings* bytecodeSettings, IOSettings* outputSettings, Object* heap, Table* stringsTable);

#endif