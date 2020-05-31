#include "IndentManagerFunctions.h"
#include "../ScannerError/ScannerError.h"

static bool PopIndent(IndentManager* manager, int spaceCount) {
    if (manager->indentStack[manager->indentStackIndex - 1] > spaceCount) {
        manager->indentStackIndex--;
        return true;
    }
    return false;
}

static int PeekIndent(IndentManager* manager) {
    return manager->indentStack[manager->indentStackIndex - 1];
}

static void PushIndent(IndentManager* manager, int indentNumber) {
    if (manager->indentStackIndex < INDENT_STACK_MAX) {
        manager->indentStack[manager->indentStackIndex] = indentNumber;
        manager->indentStackIndex++;
    }
    else {
        ScanError(ScanningIndentError);
    }
}

void InitIndentManager(IndentManager* manager) {
    manager->indentStackIndex = 0;
    manager->spaceCount = 0;
    manager->newLine = false;
    PushIndent(manager, 0);
}

IndentType GetIndentType(Scanner* scanner) {
    int spaceCount = 0;
    bool completed = false;
    while (!completed) {
        switch (*scanner->current) {
        case '\t':
            spaceCount += 4;
            scanner->current++;
            break;
        case '\r':
            spaceCount = 0;
            scanner->current++;
            break;
        case ' ':
            spaceCount++;
            scanner->current++;
            break;
        case '\n':
            scanner->current++;
            spaceCount = 0;
            break;
        default:
            completed = true;
        }
    }

    scanner->indentManager.spaceCount += spaceCount;
    int difference = scanner->indentManager.spaceCount - PeekIndent(&scanner->indentManager);
    if (difference == 0) {
        scanner->indentManager.newLine = false;
        scanner->indentManager.spaceCount = 0;
        return SAME;
    }
    else if (difference > 0) {
        PushIndent(&scanner->indentManager, spaceCount);
        scanner->indentManager.newLine = false;
        scanner->indentManager.spaceCount = 0;
        return INDENT;
    }
    else {
        bool success = PopIndent(&scanner->indentManager, spaceCount);

        if (!success) {
            scanner->indentManager.newLine = false;
            scanner->indentManager.spaceCount = 0;
            return DEDENT_ERROR;
        }

        return DEDENT;
    }
}