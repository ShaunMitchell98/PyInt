#ifndef PyInt_IndentManagerFunctions_h
#define PyInt_IndentManagerFunctions_h

#include "../Scanner.h"
#include "IndentManager.h"

void InitIndentManager(IndentManager* manager);
IndentType GetIndentType(Scanner* scanner);

#endif