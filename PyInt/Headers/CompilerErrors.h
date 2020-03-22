#ifndef PyInt_Compiler_Errors_h
#define PyInt_Compiler_Errors_h

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

void Error(const char* message);
void SyntaxError(Token* token);
#endif
