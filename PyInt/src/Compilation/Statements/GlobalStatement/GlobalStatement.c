#include "GlobalStatement.h"
#include "../../Bytecode/CompilerBytecode.h"
#include "../../Helpers/Helpers.h"
#include "../../../Services/Table/TableFunctions.h"

void GlobalStatement(Services* services, Bytecode* bytecode) {
    do {
        uint8_t bytecodeValueArrayAddress = StoreInBytecodeConstantsTable(bytecode, services, OBJ_VAL(CopyStringToTable(services->garbageCollector, services->stringsTable, services->parser->current.start, services->parser->current.length)));
        WriteBytes(bytecode, services, SET_GLOBAL_OP, bytecodeValueArrayAddress);
        GetNextToken(services);
    } while (MatchToken(services, services->parser->current, COMMA_TOKEN));
}