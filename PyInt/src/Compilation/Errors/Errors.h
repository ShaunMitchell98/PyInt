#ifndef PyInt_Compiler_Errors_h
#define PyInt_Compiler_Errors_h

#include "../../Virtual Machine/Settings/Settings.h"
#include "../../Headers/Token.h"

#define ColonError "Colon expected"
#define CommaError "Comma expected"
#define SemicolonError "Semicolon expected"
#define LeftParenError "Left bracket expected"
#define RightParenError "Right bracket expected"
#define IndentError "Indent expected"
#define InError "'in' expected"
#define ExceptError "'except' expected"
#define NameError "Name expected"
#define NewlineError "Newline expected"
#define GlobalError "Global Error"
#define ClassIdentifierError "Expect class name"
#define ClassLeftBraceError "Expect '{' after class body"
#define ClassRightBraceError "Expect '}' after class body"
#define DotCallError "Expect property name after '.'"

void Error(const char* message);
void SyntaxError(IOSettings* outputSettings, Token* token);
#endif
