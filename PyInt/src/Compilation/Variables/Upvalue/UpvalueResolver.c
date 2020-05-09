#include <string.h>

#include "CompilerUpvalue.h"
#include "UpvalueResolver.h"
#include "../../Expressions/Expressions.h"
#include "../../Compiler/Compiler.h"
#include "../../ParseFunctions/ParseFunctions.h"
#include "../Global/Global.h"
#include "../Local/LocalFunctions.h"
#include "../../Errors/Errors.h"

static int CreateOrGetUpvalue(Compiler* compiler, uint8_t localStackOffset, bool isLocal) {
    int upvalueCount = compiler->function->upvalueCount;

    for (int i = 0; i < upvalueCount; i++) {
        CompilerUpvalue* upvalue = &compiler->upvalues[i];
        if (upvalue->index == localStackOffset && upvalue->isLocal == isLocal) {
            return i;
        }
    }

    if (upvalueCount == UINT8_COUNT) {
        Error("Too many closure variables in function.");
        return 0;
    }

    compiler->upvalues[upvalueCount].isLocal = isLocal;
    compiler->upvalues[upvalueCount].index = localStackOffset;
    return compiler->function->upvalueCount++;
}

Token* GetUpvalueToken(Compiler* compiler, int upvalueIndex, bool isLocal) {
    if (compiler == NULL) {
        return NULL;
    }
    else {
        CompilerUpvalue* upvalue = &compiler->upvalues[upvalueIndex];
        if (upvalue->isLocal) {
            return &compiler->locals[upvalue->index].name;
        }
        return GetUpvalueToken((struct Compiler*)compiler->enclosing, upvalue->index, upvalue->isLocal);
    }
}

int ResolveUpvalue(Compiler* compiler, Services* services, Token* token) {
    if (compiler->enclosing == NULL) return -1;

    int localStackOffset = GetLocalStackOffset(compiler->locals, compiler->localCount, &services->parser->previous);
    if (localStackOffset != -1) {
        Compiler* enclosing = (Compiler*) compiler->enclosing;
        enclosing->locals[localStackOffset].isCaptured = true;
        return CreateOrGetUpvalue(compiler, (uint8_t)localStackOffset, true);
    }

    int upvalueIndex = ResolveUpvalue((Compiler*)compiler->enclosing, services, token);
    if (upvalueIndex != -1) {
        return CreateOrGetUpvalue(compiler, (uint8_t)upvalueIndex, false);
    }

    return -1;
}