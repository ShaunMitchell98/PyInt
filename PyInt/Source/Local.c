#include <string.h>

#include "../Headers/Local.h"
#include "../Headers/CompilerErrors.h"
#include "../Headers/Token.h"
#include "../Headers/Bytecode.h"
#include "../Headers/CompilerBytecode.h"
#include "../Headers/Object.h"
#include "../Headers/Helpers.h"
#include "../Headers/Compiler.h"

uint8_t IdentifierConstant(Compiler* compiler, Token* name) {
    return StoreConstant(compiler, OBJ_VAL(CopyString(name->start, name->length)));
}

static bool IdentifiersEqual(Token* a, Token* b) {
        if (a->length != b->length) return false;
        return memcmp(a->start, b->start, a->length) == 0;
}

int ResolveLocal(Compiler* compiler, Token* name) {
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

static int AddLocal(Compiler* compiler, Token* token) {
    if (compiler->localCount == UINT8_COUNT) {
        Error("Too many local variables in function.");
        return -1;
    }

    Local* local = &compiler->locals[compiler->localCount++];
    local->name = *token;
    local->depth = -1;
    return compiler->localCount-1;
}

static bool LocalDeclared(Compiler* compiler, Token* token) {
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

 int DeclareVariable(Compiler* compiler) {
    //Global variables are implicitly declared.
     if (compiler->scopeDepth == 0) return -1;
    
    Token* token = &compiler->parser->previous;
    if (!LocalDeclared(compiler, token)) {
        return AddLocal(compiler, token);
    }
     return -1;
}

uint8_t ParseVariable(Compiler* compiler) {
    int address = DeclareVariable(compiler);
    
    if (address != -1){
        WriteBytes(compiler, DECLARE_LOCAL_OP, address);
    }
    
    if (compiler->scopeDepth > 0) return 0;
    return IdentifierConstant(compiler, &compiler->parser->previous);
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

void MarkInitialised(Compiler* compiler) {
    if (compiler->scopeDepth == 0) return;
    compiler->locals[compiler->localCount-1].depth = compiler->scopeDepth;
}

void DefineVariable(Compiler* compiler, uint8_t global) {
    if (compiler->scopeDepth > 0) {
        MarkInitialised(compiler);
        WriteBytes(compiler, SET_LOCAL_OP, (uint8_t)compiler->localCount-1);
        return;
    }
    WriteBytes(compiler, DEFINE_GLOBAL_OP, global);
}
