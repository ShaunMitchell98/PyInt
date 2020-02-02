#include <string.h>

#include "Local.h"
#include "CompilerErrors.h"
#include "Token.h"
#include "Bytecode.h"
#include "CompilerBytecode.h"
#include "Object.h"
#include "Helpers.h"
#include "Compiler.h"

uint8_t IdentifierConstant(Token* name) {
    return MakeConstant(OBJ_VAL(CopyString(name->start, name->length)));
}

static bool IdentifiersEqual(Token* a, Token* b) {
        if (a->length != b->length) return false;
        return memcmp(a->start, b->start, a->length) == 0;
}

int ResolveLocal(Token* name) {
    for (int i = compiler.localCount-1; i >= 0; i--) {
        Local* local = &compiler.locals[i];
        if (IdentifiersEqual(name, &local->name)) {
            if(local->depth == -1) {
                Error("Cannot read local variable in its own initializer.");
            }
            return i;
        }
    }

    return -1;
}

static void AddLocal(Token* token) {
    if (compiler.localCount == UINT8_COUNT) {
        Error("Too many local variables in function.");
        return;
    }

    Local* local = &compiler.locals[compiler.localCount++];
    local->name = *token;
    local->depth = -1;
}

bool LocalDeclared(Token* token) {
    for (int i = compiler.localCount-1; i >=0; i--) {
        Local* local = &compiler.locals[i];
        if (local->depth != -1 && local->depth < compiler.scopeDepth) {
            return false;
        }

        if (IdentifiersEqual(token, &local->name)) {
            Error("Variable with this name already declared in this scope.");
            return true;
        }
    }
    
    return false;
}

void DeclareVariable() {
    //Global variables are implicitly declared.
    if (compiler.scopeDepth == 0) return;
    
    Token* token = &parser.previous;
    if (!LocalDeclared(token)) {
        AddLocal(token);
    }
}

uint8_t ParseVariable(const char* errorMessage) {
    ConsumeToken(IDENTIFIER_TOKEN, errorMessage);

    DeclareVariable();
    if (compiler.scopeDepth > 0) return 0;
    return IdentifierConstant(&parser.previous);
}

void BeginScope() {
    compiler.scopeDepth++;
}

void EndScope() {
    compiler.scopeDepth--;
    
    while (compiler.localCount > 0 && compiler.locals[compiler.localCount-1].depth > compiler.scopeDepth) {
        WriteByte(POP_OP);
        compiler.localCount--;
    }
}

static void MarkInitialized(Compiler* compiler) {
    if (compiler->scopeDepth == 0) return;
    compiler->locals[compiler->localCount-1].depth = compiler->scopeDepth;
}

static void DefineVariable(Compiler* compiler, uint8_t global) {
    if (compiler->scopeDepth > 0) {
        MarkInitialized(compiler);
        return;
    }
    WriteBytes(DEFINE_GLOBAL_OP, global);
}
