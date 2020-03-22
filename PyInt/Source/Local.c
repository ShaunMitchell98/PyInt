#include <string.h>

#include "../Headers/Local.h"
#include "../Headers/CompilerErrors.h"
#include "../Headers/Token.h"
#include "../Headers/Bytecode.h"
#include "../Headers/CompilerBytecode.h"
#include "../Headers/Object.h"
#include "../Headers/Helpers.h"
#include "../Headers/Compiler.h"

uint8_t IdentifierConstant(Token* name) {
    return StoreConstant(OBJ_VAL(CopyString(name->start, name->length)));
}

static bool IdentifiersEqual(Token* a, Token* b) {
        if (a->length != b->length) return false;
        return memcmp(a->start, b->start, a->length) == 0;
}

int ResolveLocal(Token* name) {
    for (int i = currentCompiler->localCount-1; i >= 0; i--) {
        Local* local = &currentCompiler->locals[i];
        if (IdentifiersEqual(name, &local->name)) {
            if(local->depth == -1) {
                Error("Cannot read local variable in its own initializer.");
            }
            return i;
        }
    }

    return -1;
}

static int AddLocal(Token* token) {
    if (currentCompiler->localCount == UINT8_COUNT) {
        Error("Too many local variables in function.");
        return -1;
    }

    Local* local = &currentCompiler->locals[currentCompiler->localCount++];
    local->name = *token;
    local->depth = -1;
    return currentCompiler->localCount-1;
}

bool LocalDeclared(Token* token) {
    for (int i = currentCompiler->localCount-1; i >=0; i--) {
        Local* local = &currentCompiler->locals[i];
        if (local->depth != -1 && local->depth < currentCompiler->scopeDepth) {
            return false;
        }

        if (IdentifiersEqual(token, &local->name)) {
            Error("Variable with this name already declared in this scope.");
            return true;
        }
    }
    
    return false;
}

 int DeclareVariable() {
    //Global variables are implicitly declared.
     if (currentCompiler->scopeDepth == 0) return -1;
    
    Token* token = &parser.previous;
    if (!LocalDeclared(token)) {
        return AddLocal(token);
    }
     return -1;
}

uint8_t ParseVariable() {
    int address = DeclareVariable();
    
    if (address != -1){
        WriteBytes(DECLARE_LOCAL_OP, address);
    }
    
    if (currentCompiler->scopeDepth > 0) return 0;
    return IdentifierConstant(&parser.previous);
}

void BeginScope() {
    currentCompiler->scopeDepth++;
}

void EndScope() {
    currentCompiler->scopeDepth--;
    
    while (currentCompiler->localCount > 0 && currentCompiler->locals[currentCompiler->localCount-1].depth > currentCompiler->scopeDepth) {
        WriteByte(POP_OP);
        currentCompiler->localCount--;
    }
}

void MarkInitialised() {
    if (currentCompiler->scopeDepth == 0) return;
    currentCompiler->locals[currentCompiler->localCount-1].depth = currentCompiler->scopeDepth;
}

void DefineVariable(uint8_t global) {
    if (currentCompiler->scopeDepth > 0) {
        MarkInitialised();
        WriteBytes(SET_LOCAL_OP, (uint8_t)currentCompiler->localCount-1);
        return;
    }
    WriteBytes(DEFINE_GLOBAL_OP, global);
}
