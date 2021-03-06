#ifndef PyInt_Token_h
#define PyInt_Token_h

typedef enum {
    LEFT_PAREN_TOKEN,
    RIGHT_PAREN_TOKEN,
    LEFT_BRACE_TOKEN,
    RIGHT_BRACE_TOKEN,
    EQUAL_TOKEN,
    NOT_EQUAL_TOKEN,
    EQUAL_EQUAL_TOKEN,
    GREATER_TOKEN,
    GREATER_EQUAL_TOKEN,
    LESSER_TOKEN,
    LESSER_EQUAL_TOKEN,
    SEMICOLON_TOKEN,
    COMMA_TOKEN,
    DOT_TOKEN,

    PLUS_TOKEN,
    MINUS_TOKEN,
    STAR_TOKEN,
    SLASH_TOKEN,
 
    NUMBER_TOKEN,
    STRING_TOKEN,
    EOF_TOKEN,
    IF_TOKEN,
    ELIF_TOKEN,
    ELSE_TOKEN,
    FOR_TOKEN,
    IN_TOKEN,
    WHILE_TOKEN,
    
    ERROR_TOKEN,
    LAMBDA_TOKEN,
    AND_TOKEN,
    OR_TOKEN,
    NOT_TOKEN,
    TRY_TOKEN,
    WITH_TOKEN,
    DEF_TOKEN,
    CLASS_TOKEN,
    COLON_TOKEN,
    COLON_EQUAL_TOKEN,
    
    INDENT_TOKEN,
    NEWLINE_TOKEN,
    DEDENT_TOKEN,
    TYPE_COMMENT_TOKEN,
    EXCEPT_TOKEN,
    AS_TOKEN,
    NAME_TOKEN,
    TRUE_TOKEN,
    FALSE_TOKEN,
    GLOBAL_TOKEN,
    IDENTIFIER_TOKEN,
    POWER_TOKEN,
    RETURN_TOKEN,
    PASS_TOKEN
} TokenType;

typedef struct {    
    const char* start;
    int length;
    int line;
    TokenType type;
} Token;

#endif
