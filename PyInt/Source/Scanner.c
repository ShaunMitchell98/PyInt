#include <stdio.h>
#include  <stddef.h>

#include "../Headers/Scanner.h"
#include "../Headers/Common.h"
#include "../Headers/Compiler.h"

static void ScanError(const char* message) {
    fprintf(stderr, "%s\n", message);
}

static void PushIndent(Scanner* scanner, int indentNumber) {
    if (scanner->stackIndex < INDENT_STACK_MAX){
        scanner->indentStack[scanner->stackIndex] = indentNumber;
        scanner->stackIndex++;
    }
    else {
        ScanError(ScanningIndentError);
    }
}

static int PopIndent(Scanner* scanner, int spaceCount) {
    int dedentCount;
    for (int i = 0; i < scanner->stackIndex; i++) {
        if (scanner->indentStack[i] == spaceCount) {
            dedentCount = scanner->stackIndex -1;
            scanner->stackIndex -= 1;
            return dedentCount;
        }
    }
    return -1;
}

static int PeekIndent(Scanner* scanner) {
    return scanner->indentStack[scanner->stackIndex-1];
}

void InitScanner(Scanner* scanner, const char* sourceCode) {
    scanner->start = sourceCode;
    scanner->current = sourceCode;
    scanner->stackIndex = 0;
    scanner->line = 1;
    scanner->newLine = true;
    PushIndent(scanner, 0);
    return;
}

static Token MakeToken(Scanner* scanner, TokenType tokenType) {
    Token token;
    token.start = scanner->start;
    token.length = (int) (scanner->current - scanner->start);
    token.type = tokenType;
    token.line = scanner->line;
    return token;
}

static bool IsDigit(Scanner* scanner, char _char) {
    return (_char >= '0' && _char <= '9') || (_char == '.' && scanner->current != scanner->start);
}

static bool IsAlpha (char _char) {
    return (_char >= 'a' && _char <= 'z') || (_char >= 'A' && _char <= 'Z');
}
    
static bool CheckKeyword(Scanner* scanner, const char* letters, int length) {
    for (int i = 1; i <= length; i++) {
        if (scanner->start[i] != letters[i-1]) {
            return false;
        }
    }
    return true;
}

static Token Identifier(Scanner* scanner) {
    while (IsAlpha(*scanner->current) || IsDigit(scanner, *scanner->current) || *scanner->current == '_') {
        scanner->current++;
    }
    
    switch(scanner->start[0]) {
        case 'a': {
            if (CheckKeyword(scanner, "nd", 2)) {
                return MakeToken(scanner, AND_TOKEN);
            }
            else {
                return MakeToken(scanner, IDENTIFIER_TOKEN);
            }
        }
        case 'd': {
            if (CheckKeyword(scanner, "ef", 2)) {
                return MakeToken(scanner, DEF_TOKEN);
            }
            else {
                return MakeToken(scanner, IDENTIFIER_TOKEN);
            }
        }
        case 'e': {
               if (CheckKeyword(scanner, "lse", 3)) {
                   return MakeToken(scanner, ELSE_TOKEN);
               }
               else if (CheckKeyword(scanner, "lif", 3)) {
                   return MakeToken(scanner, ELIF_TOKEN);
               }
               else {
                   return MakeToken(scanner, IDENTIFIER_TOKEN);
               }
           }
            case 'i': {
                if (CheckKeyword(scanner, "f", 1)) {
                    return MakeToken(scanner, IF_TOKEN);
                }
                else if (CheckKeyword(scanner, "n", 1)) {
                    return MakeToken(scanner, IN_TOKEN);
                }
                else {
                    return MakeToken(scanner, IDENTIFIER_TOKEN);
                }
            }
            case 'f': {
                if (CheckKeyword(scanner, "alse", 4)) {
                    return MakeToken(scanner, FALSE_TOKEN);
                }
                else if (CheckKeyword(scanner, "or", 2)) {
                    return MakeToken(scanner, FOR_TOKEN);
                }
                else {
                    return MakeToken(scanner, IDENTIFIER_TOKEN);
                }
            }
            case 'g': {
                if (CheckKeyword(scanner, "lobal", 5)) {
                    return MakeToken(scanner, GLOBAL_TOKEN);
                }
                else {
                    return MakeToken(scanner, IDENTIFIER_TOKEN);
                }
            }
        case 'n': {
            if (CheckKeyword(scanner, "ot", 2)) {
                return MakeToken(scanner, NOT_TOKEN);
            }
            else {
                return MakeToken(scanner, IDENTIFIER_TOKEN);
            }
        }
            case 'o': {
                if (CheckKeyword(scanner, "r", 1)) {
                    return MakeToken(scanner, OR_TOKEN);
                }
                else {
                    return MakeToken(scanner, IDENTIFIER_TOKEN);
                }
            }
            case 'p': {
                if (CheckKeyword(scanner, "rint", 4)) {
                    return MakeToken(scanner, PRINT_TOKEN);
                }
                else {
                    return MakeToken(scanner, IDENTIFIER_TOKEN);
                }
            }
            case 't': {
                if (CheckKeyword(scanner, "rue", 3)) {
                    return MakeToken(scanner, TRUE_TOKEN);
                }
                else {
                    return MakeToken(scanner, IDENTIFIER_TOKEN);
                }
            }
            case 'w': {
                if (CheckKeyword(scanner, "hile", 4)) {
                    return MakeToken(scanner, WHILE_TOKEN);
                }
                else {
                    return MakeToken(scanner, IDENTIFIER_TOKEN);
                }
            }
            default:
                return MakeToken(scanner, IDENTIFIER_TOKEN);
        }
}

static Token Number(Scanner* scanner) {
    while (IsDigit(scanner, *scanner->current)) {
        scanner->current++;
    }
    return MakeToken(scanner, NUMBER_TOKEN);
}

static Token String(Scanner* scanner, char quoteMark) {
    scanner->start++;
    while(*scanner->current != quoteMark) {
        scanner->current++;
    }
    Token token = MakeToken(scanner, STRING_TOKEN);
    scanner->current += 1;
    return token;
}

static void SkipWhiteSpace(Scanner* scanner) {
    for (;;) {
        switch(*scanner->current) {
            case '\t':
            case '\r':
            case ' ':
                scanner->current++;
                break;
            default: return;
        }
    }
}

static SpaceType CheckIndent(Scanner* scanner) {
    int spaceCount = 0;
    while (true) {
        switch(*scanner->current) {
            case '\t':
                spaceCount += 4;
                scanner->current++;
                break;
            case '\r':
                spaceCount = 0;
                break;
            case ' ':
                spaceCount++;
                scanner->current++;
                break;
            default:
                goto Label;
                
        }
    }
Label:;
    int difference = spaceCount - PeekIndent(scanner);
    if (difference == 0) {
        return SAME;
    }
    else if (difference > 0) {
        PushIndent(scanner, spaceCount);
        return INDENT;
    }
    else {
        int dedentNumber = PopIndent(scanner, spaceCount);
        
        if (dedentNumber == -1) {
            return DEDENT_ERROR;
        }
        else {
            return DEDENT;
        }
    }
}

Token GetToken(Scanner* scanner) {
    if (scanner->newLine) {
        scanner->newLine = false;
        switch(CheckIndent(scanner)) {
            case INDENT:
                return MakeToken(scanner, INDENT_TOKEN);
            case DEDENT:
                return MakeToken(scanner, DEDENT_TOKEN);
            default:
                break;
        }
    }
    else {
        SkipWhiteSpace(scanner);
    }

    scanner->start = scanner->current;
    char currentChar = *scanner->start;
      
    scanner->current++;
    
    if (IsDigit(scanner, currentChar)) return Number(scanner);
    if (IsAlpha(currentChar)) return Identifier(scanner);
    
    switch(currentChar) {
        case '.':
            return MakeToken(scanner, DOT_TOKEN);
        case ';':
            return MakeToken(scanner, SEMICOLON_TOKEN);
        case ':':
            return MakeToken(scanner, COLON_TOKEN);
        case ',':
            return MakeToken(scanner, COMMA_TOKEN);
        case '!':
            if (CheckKeyword(scanner, "=", 1)) {
                scanner->current++;
                return MakeToken(scanner, NOT_EQUAL_TOKEN);
            }
        case '\'':
            return String(scanner, '\'');
        case '"':
            return String(scanner, '"');
        case '=': {
            if (CheckKeyword(scanner, "=", 1)) {
                scanner->current++;
                return MakeToken(scanner, EQUAL_EQUAL_TOKEN);
            }
            else {
                return MakeToken(scanner, EQUAL_TOKEN);
            }
        }
        case '+':
            return MakeToken(scanner, PLUS_TOKEN);
        case '-':
            return MakeToken(scanner, MINUS_TOKEN);
        case '*': {
            if (CheckKeyword(scanner, "*", 1)) {
                scanner->current++;
                return MakeToken(scanner, POWER_TOKEN);
            }
            else {
                return MakeToken(scanner, STAR_TOKEN);
            }
        }
        case '/':
            return MakeToken(scanner, SLASH_TOKEN);
        case '>': {
            if (CheckKeyword(scanner, "=", 1)) {
                scanner->current++;
                 return MakeToken(scanner, GREATER_EQUAL_TOKEN);
            }
            else {
                return MakeToken(scanner, GREATER_TOKEN);
            }
        }
        case '<': {
            if (CheckKeyword(scanner, "=", 1)) {
                scanner->current++;
                return MakeToken(scanner, LESSER_EQUAL_TOKEN);
            }
            else {
                return MakeToken(scanner, LESSER_TOKEN);
            }
        }
        case '(':
            return MakeToken(scanner, LEFT_PAREN_TOKEN);
        case ')':
            return MakeToken(scanner, RIGHT_PAREN_TOKEN);
        case '\0':
            return MakeToken(scanner, EOF_TOKEN);
        case '\n':
            scanner->line++;
            scanner->newLine = true;
            return MakeToken(scanner, NEWLINE_TOKEN);
        default:
            return MakeToken(scanner, ERROR_TOKEN);
            
    }
}
