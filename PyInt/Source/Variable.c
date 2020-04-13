#include <string.h>

#include "../Headers/Variable.h"
#include "../Headers/CompilerErrors.h"
#include "../Headers/Token.h"
#include "../Headers/Bytecode.h"
#include "../Headers/CompilerBytecode.h"
#include "../Headers/Object.h"
#include "../Headers/Helpers.h"
#include "../Headers/Compiler.h"

uint8_t StoreGlobalInBytecodeConstants(Compiler* compiler, Token* name) {
    return StoreInBytecodeValueArray(compiler, OBJ_VAL(CopyStringToTable(compiler->vm, &compiler->vm->strings, name->start, name->length)));
}

static bool IdentifiersEqual(Token* a, Token* b) {
        if (a->length != b->length) return false;
        return memcmp(a->start, b->start, a->length) == 0;
}

int GetLocalStackOffset(Compiler* compiler, Token* name) {
    for (int i = compiler->localCount-1; i >= 0; i--) {
        Local* local = &compiler->locals[i];
        if (IdentifiersEqual(name, &local->name)) {
            if(local->depth == -1) {
                Error("Cannot read local variable in its own initializer.");
            }
            return i;
        }
    }

    return -1;
}

static int AddLocalToCompilerStack(Compiler* compiler, Token* token) {
    if (compiler->localCount == UINT8_COUNT) {
        Error("Too many local variables in function.");
        return -1;
    }

    Local* local = &compiler->locals[compiler->localCount++];
    local->name = *token;
    local->depth = compiler->scopeDepth;
    return compiler->localCount-1;
}

static bool IsLocalInCompilerStack(Compiler* compiler, Token* token) {
    for (int i = compiler->localCount-1; i >=0; i--) {
        Local* local = &compiler->locals[i];
        if (local->depth != -1 && local->depth < compiler->scopeDepth) {
            return false;
        }

        if (IdentifiersEqual(token, &local->name)) {
            Error("Variable with this name already declared in this scope.");
            return true;
        }
    }
    
    return false;
}

 int PushLocalToCompilerStack(Compiler* compiler) {
    Token* token = &compiler->parser->current;
    if (!IsLocalInCompilerStack(compiler, token)) {
        return AddLocalToCompilerStack(compiler, token);
    }

    return GetLocalStackOffset(compiler, token);
}

 void SetLocalVariable(Compiler* compiler) {
     int stackOffset = PushLocalToCompilerStack(compiler);
     WriteBytes(compiler, SET_LOCAL_OP, stackOffset);
 }

 void SetGlobalVariable(Compiler* compiler) {
     uint8_t bytecodeConstantAddress = StoreGlobalInBytecodeConstants(compiler, &compiler->parser->current);
     WriteBytes(compiler, SET_GLOBAL_OP, bytecodeConstantAddress);
 }

void BeginScope(Compiler* compiler) {
    compiler->scopeDepth++;
}

void EndScope(Compiler* compiler) {
    compiler->scopeDepth--;
    
    while (compiler->localCount > 0 && compiler->locals[compiler->localCount-1].depth > compiler->scopeDepth) {
        WriteByte(compiler, POP_OP);
        compiler->localCount--;
    }
}

uint8_t ArguementList(Compiler* compiler) {
    uint8_t argCount = 0;
    if (!CheckToken(compiler, RIGHT_PAREN_TOKEN)) {
        do {
            Expression(compiler);

            if (argCount == 255) {
                Error("Cannot have more than 255 arguements");
            }

            argCount++;
        } while (MatchToken(compiler, compiler->parser->current, COMMA_TOKEN));
    }

    ConsumeToken(compiler, RIGHT_PAREN_TOKEN, "Expect ')' after arguements");
    return argCount;
}
