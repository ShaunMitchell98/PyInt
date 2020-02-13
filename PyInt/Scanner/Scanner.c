#include <stdio.h>

#include "Scanner.h"
#include "stddef.h"
#include "Common.h"
#include "Compiler.h"

Scanner scanner;

static void ScanError(const char* message) {
    fprintf(stderr, "%s\n", message);
}

static void PushIndent(int indentNumber) {
    if (scanner.stackIndex < INDENT_STACK_MAX){
        scanner.indentStack[scanner.stackIndex] = indentNumber;
        scanner.stackIndex++;
    }
    else {
        ScanError(ScanningIndentError);
    }
}

static int PopIndent(int spaceCount) {
    int dedentCount;
    for (int i = 0; i < scanner.stackIndex; i++) {
        if (scanner.indentStack[i] == spaceCount) {
            dedentCount = scanner.stackIndex -1;
            scanner.stackIndex -= 1;
            return dedentCount;
        }
    }
    return -1;
}

static int PeekIndent() {
    return scanner.indentStack[scanner.stackIndex-1];
}

void InitScanner(const char* sourceCode, const char* path) {
    scanner.path = path;
    scanner.start = sourceCode;
    scanner.current = sourceCode;
    scanner.stackIndex = 0;
    scanner.line = 1;
    scanner.newLine = true;
    PushIndent(0);
    return;
}

static Token MakeToken(TokenType tokenType) {
    Token token;
    token.tokenPath = scanner.path;
    token.start = scanner.start;
    token.length = (int) (scanner.current - scanner.start);
    token.type = tokenType;
    token.line = scanner.line;
    return token;
}

static bool IsDigit(char _char) {
    return (_char >= '0' && _char <= '9') || (_char == '.' && scanner.current != scanner.start);
}

static bool IsAlpha (char _char) {
    return (_char >= 'a' && _char <= 'z') || (_char >= 'A' && _char <= 'Z');
}

static bool CheckKeyword(const char* letters, int length) {
    for (int i = 1; i <= length; i++) {
        if (scanner.start[i] != letters[i-1]) {
            return false;
        }
    }
    return true;
}

static Token Identifier() {
    while (IsAlpha(*scanner.current) || IsDigit(*scanner.current) || *scanner.current == '_') {
        scanner.current++;
    }
    
    switch(scanner.start[0]) {
        case 'a': {
            if (CheckKeyword("nd", 2)) {
                return MakeToken(AND_TOKEN);
            }
            else {
                return MakeToken(IDENTIFIER_TOKEN);
            }
        }
        case 'e': {
               if (CheckKeyword("lse", 3)) {
                   return MakeToken(ELSE_TOKEN);
               }
               else if (CheckKeyword("lif", 3)) {
                   return MakeToken(ELIF_TOKEN);
               }
               else {
                   return MakeToken(IDENTIFIER_TOKEN);
               }
           }
            case 'i': {
                if (CheckKeyword("f", 1)) {
                    return MakeToken(IF_TOKEN);
                }
                else {
                    return MakeToken(IDENTIFIER_TOKEN);
                }
            }
            case 'f': {
                if (CheckKeyword("alse", 4)) {
                    return MakeToken(FALSE_TOKEN);
                }
                else {
                    return MakeToken(IDENTIFIER_TOKEN);
                }
            }
            case 'g': {
                if (CheckKeyword("lobal", 5)) {
                    return MakeToken(GLOBAL_TOKEN);
                }
                else {
                    return MakeToken(IDENTIFIER_TOKEN);
                }
            }
        case 'n': {
            if (CheckKeyword("ot", 2)) {
                return MakeToken(NOT_TOKEN);
            }
            else {
                return MakeToken(IDENTIFIER_TOKEN);
            }
        }
            case 'o': {
                if (CheckKeyword("r", 1)) {
                    return MakeToken(OR_TOKEN);
                }
                else {
                    return MakeToken(IDENTIFIER_TOKEN);
                }
            }
            case 'p': {
                if (CheckKeyword("rint", 4)) {
                    return MakeToken(PRINT_TOKEN);
                }
                else {
                    return MakeToken(IDENTIFIER_TOKEN);
                }
            }
            case 't': {
                if (CheckKeyword("rue", 3)) {
                    return MakeToken(TRUE_TOKEN);
                }
                else {
                    return MakeToken(IDENTIFIER_TOKEN);
                }
            }
            case 'w': {
                if (CheckKeyword("hile", 4)) {
                    return MakeToken(WHILE_TOKEN);
                }
                else {
                    return MakeToken(IDENTIFIER_TOKEN);
                }
            }
            default:
                return MakeToken(IDENTIFIER_TOKEN);
        }
}

static Token Number() {
    while (IsDigit(*scanner.current)) {
        scanner.current++;
    }
    return MakeToken(NUMBER_TOKEN);
}

static void SkipWhiteSpace() {
    for (;;) {
        switch(*scanner.current) {
            case '\t':
            case '\r':
            case ' ':
                scanner.current++;
                break;
            default: return;
        }
    }
}

static SpaceType CheckIndent() {
    int spaceCount = 0;
    while (true) {
        switch(*scanner.current) {
            case '\t':
                spaceCount += 4;
                scanner.current++;
                break;
            case '\r':
                spaceCount = 0;
                break;
            case ' ':
                spaceCount++;
                scanner.current++;
                break;
            default:
                goto Label;
                
        }
    }
Label:;
    int difference = spaceCount - PeekIndent();
    if (difference == 0) {
        return SAME;
    }
    else if (difference > 0) {
        PushIndent(spaceCount);
        return INDENT;
    }
    else {
        int dedentNumber = PopIndent(spaceCount);
        
        if (dedentNumber == -1) {
            return DEDENT_ERROR;
        }
        else {
            return DEDENT;
        }
    }
}

Token GetToken() {
    if (scanner.newLine) {
        scanner.newLine = false;
        switch(CheckIndent()) {
            case INDENT:
                return MakeToken(INDENT_TOKEN);
            case DEDENT:
                return MakeToken(DEDENT_TOKEN);
            default:
                break;
        }
    }
    else {
        SkipWhiteSpace();
    }

    scanner.start = scanner.current;
    char currentChar = *scanner.start;
      
    scanner.current++;
    
    if (IsDigit(currentChar)) return Number();
    if (IsAlpha(currentChar)) return Identifier();
    
    switch(currentChar) {
        case '.':
            return MakeToken(DOT_TOKEN);
        case ';':
            return MakeToken(SEMICOLON_TOKEN);
        case ':':
            return MakeToken(COLON_TOKEN);
        case ',':
            return MakeToken(COMMA_TOKEN);
        case '=': {
            if (CheckKeyword("=", 1)) {
                scanner.current++;
                return MakeToken(EQUAL_EQUAL_TOKEN);
            }
            else {
                return MakeToken(EQUAL_TOKEN);
            }
        }
        case '+':
            return MakeToken(PLUS_TOKEN);
        case '-':
            return MakeToken(MINUS_TOKEN);
        case '*': {
            if (CheckKeyword("*", 1)) {
                scanner.current++;
                return MakeToken(POWER_TOKEN);
            }
            else {
                return MakeToken(STAR_TOKEN);
            }
        }
        case '/':
            return MakeToken(SLASH_TOKEN);
        case '>': {
            if (CheckKeyword("=", 1)) {
                scanner.current++;
                 return MakeToken(GREATER_EQUAL_TOKEN);
            }
            else {
                return MakeToken(GREATER_TOKEN);
            }
        }
        case '<': {
            if (CheckKeyword("=", 1)) {
                scanner.current++;
                return MakeToken(LESSER_EQUAL_TOKEN);
            }
            else {
                return MakeToken(LESSER_TOKEN);
            }
        }
        case '(':
            return MakeToken(LEFT_PAREN_TOKEN);
        case ')':
            return MakeToken(RIGHT_PAREN_TOKEN);
        case '\0':
            return MakeToken(EOF_TOKEN);
        case '\n':
            scanner.line++;
            scanner.newLine = true;
            return MakeToken(NEWLINE_TOKEN);
        default:
            return MakeToken(ERROR_TOKEN);
            
    }
}
