#include <string.h>

#include "Local.h"
#include "LocalFunctions.h"
#include "../../Bytecode/CompilerBytecode.h"
#include "../../Errors/Errors.h"

static bool IdentifiersEqual(Token* a, Token* b) {
    if (a->length != b->length) return false;
    return memcmp(a->start, b->start, a->length) == 0;
}

static int AddLocalToCompilerStack(Compiler* compiler, Token* token) {
    if (compiler->localCount == UINT8_COUNT) {
        Error("Too many local variables in function.");
        return -1;
    }

    Local* local = compiler->locals + compiler->localCount;
    local->name = *token;
    local->isCaptured = false;
    return compiler->localCount++;
}

static bool IsLocalInCompilerStack(Local* locals, int localCount, Token* token) {
    for (int i = localCount - 1; i >= 0; i--) {
        Local* local = locals + i;

        if (IdentifiersEqual(token, &local->name)) {
            Error("Variable with this name already declared in this scope.");
            return true;
        }
    }

    return false;
}

int GetLocalStackOffset(Local* locals, int localCount, Token* token) {
    for (int i = localCount - 1; i >= 0; i--) {
        Local* local = (locals + i);
        if (IdentifiersEqual(token, &local->name)) {
            return i;
        }
    }

    return -1;
}

int PushLocalToCompilerStack(Compiler* compiler, Token* token) {
    if (!IsLocalInCompilerStack(compiler->locals, compiler->localCount, token)) {
        return AddLocalToCompilerStack(compiler, token);
    }

    return GetLocalStackOffset(compiler->locals, compiler->localCount, token);
}

void SetLocalVariable(Compiler* compiler, Bytecode* bytecode, Services* services) {
    int stackOffset = PushLocalToCompilerStack(compiler, &services->parser->previous);
    WriteBytes(bytecode, services, SET_LOCAL_OP, stackOffset);
}
