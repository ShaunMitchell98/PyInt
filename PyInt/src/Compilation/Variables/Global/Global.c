#include "Global.h"
#include "../../Bytecode/CompilerBytecode.h"
#include "../../../Services/Table/Table.h"
#include "../../../Services/Table/TableFunctions.h"

uint8_t StoreGlobalInBytecodeConstantTable(Bytecode* bytecode, Services* services, Token* name) {
    return StoreInBytecodeConstantsTable(bytecode, services, OBJ_VAL(CopyStringToTable(services->garbageCollector, services->stringsTable, name->start, name->length)));
}

void SetGlobalVariable(Bytecode* bytecode, Services* services) {
    uint8_t bytecodeConstantAddress = StoreGlobalInBytecodeConstantTable(bytecode, services, &services->parser->current);
    WriteBytes(bytecode, services, SET_GLOBAL_OP, bytecodeConstantAddress);
}