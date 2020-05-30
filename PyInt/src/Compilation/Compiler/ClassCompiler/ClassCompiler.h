#ifndef PyInt_ClassCompiler_h
#define PyInt_ClassCompiler_h

#include "../Compiler.h"
#include "../../../Types/Class/Class.h"
#include "../../../Headers/Token.h"
#include "../../../Types/Class/Class.h"

typedef struct {
    Compiler compiler;
    Class* klass;
    Token name;
    String* superclassName;
} ClassCompiler;

#endif