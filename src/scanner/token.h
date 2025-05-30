/**
 * @name Implementation of IFJ23 language compiler
 * @file token.h
 * @brief header file that contains different types of token and
 * structures for necessary token data and token values to store
 *
 * @author Denis Milistenfer (xmilis00)
 */

#ifndef TOKEN_H
#define TOKEN_H

// TokenTypes: all possible token types that scanner generates
typedef enum {
    TOKEN_IDENTIF,
    TOKEN_KEYWORD_DOUBLE,        // Double
    TOKEN_KEYWORD_DOUBLE_NIL,    // Double?
    TOKEN_KEYWORD_ELSE,          // else
    TOKEN_KEYWORD_FUNC,          // func
    TOKEN_KEYWORD_IF,            // if
    TOKEN_KEYWORD_INT,           // Int
    TOKEN_KEYWORD_INT_NIL,       // Int?
    TOKEN_KEYWORD_LET,           // let
    TOKEN_KEYWORD_NIL,           // nil
    TOKEN_KEYWORD_RETURN,        // return
    TOKEN_KEYWORD_STRING,        // String
    TOKEN_KEYWORD_STRING_NIL,    // String?
    TOKEN_KEYWORD_VAR,           // var
    TOKEN_KEYWORD_WHILE,         // while
    TOKEN_KEYWORD_BOOL,          // Bool
    TOKEN_KEYWORD_BOOL_NIL,      // Bool?
    TOKEN_BOOL,                  //true/false
    TOKEN_INT,                   // Int
    TOKEN_FLOAT,                 //Float
    TOKEN_STRING,                // e.g. "any string"
    TOKEN_ADD,                   // +
    TOKEN_SUB,                   // -
    TOKEN_MUL,                   // *
    TOKEN_DIV,                   // /
    TOKEN_NIL_COALESCING,        // ??
    TOKEN_EXCL_MARK,             // !
    TOKEN_NOT,                   // !xxx
    TOKEN_NOT_EQUAL,             // !=
    TOKEN_EQUAL,                 // ==
    TOKEN_ASSIGN,                // =
    TOKEN_GREATER,               // >
    TOKEN_GREATER_EQUAL,         // >=
    TOKEN_LESSER,                // <
    TOKEN_LESSER_EQUAL,          // <=     
    TOKEN_L_BRACKET,             // (
    TOKEN_R_BRACKET,             // )
    TOKEN_L_CRLY_BRACKET,        // {
    TOKEN_R_CRLY_BRACKET,        // }
    TOKEN_COLON,                 // :
    TOKEN_COMA,                  // ,
    TOKEN_ARROW,                 // ->
    TOKEN_UNDERSCORE,            // _
    TOKEN_AND,                   // &&
    TOKEN_OR,                    // ||
    TOKEN_END_OF_FILE
} TokenType;

// TokenValue: structure to store string, identifier, number or boolean values
typedef union {
    char* string;
    long long int int_value;
    double float_value;
    bool boolean;
} TokenValue;

// TokenData: structure to store all neccesary data for token
typedef struct {
    TokenType type;
    int id;
    int line_index;
    bool eol_before;
    TokenValue value;
} TokenData;

#endif /* TOKEN_H */
