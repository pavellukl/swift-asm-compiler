#ifndef TOKEN_H
#define TOKEN_H

typedef enum {
    TOKEN_LINE_COMMENT,         
    TOKEN_BLOCK_COMMENT,                                   // TODO: nested comments
    TOKEN_IDENTIF,
    TOKEN_KEYWORD_INT,          // Int
    TOKEN_KEYWORD_INT_NIL,      // Int?
    TOKEN_KEYWORD_DOUBLE,       // Double
    TOKEN_KEYWORD_DOUBLE_NIL,   // Double?
    TOKEN_KEYWORD_STRING,       // String
    TOKEN_KEYWORD_STRING_NIL,   // String?
    TOKEN_KEYWORD_NIL,          // nil
    TOKEN_KEYWORD_LET,          // let
    TOKEN_KEYWORD_VAR,          // var
    TOKEN_KEYWORD_FUNC,         // func
    TOKEN_KEYWORD_IF,           // if
    TOKEN_KEYWORD_ELSE,         // else
    TOKEN_KEYWORD_WHILE,        // while
    TOKEN_KEYWORD_RETURN,       // return
    TOKEN_FUNC_READ_INT,        // readInt
    TOKEN_FUNC_READ_DOUBLE,     // readDouble
    TOKEN_FUNC_READ_STRING,     // readString
    TOKEN_FUNC_INT_2_DOUBLE,    // Int2Double
    TOKEN_FUNC_DOUBLE_2_INT,    // Double2Int
    TOKEN_FUNC_WRITE,           // write
    TOKEN_FUNC_LENGTH,          // length
    TOKEN_FUNC_SUBSTRING,       // substring
    TOKEN_FUNC_ORD,             // ord
    TOKEN_FUNC_CHR,             // chr
    TOKEN_NUM_W_EXP,            // e.g. 1.23E+10
    TOKEN_INT_NUM,              // e.g. 6
    TOKEN_FLOAT_NUM,            // e.g. 1.2                // TODO: nested strings
    TOKEN_STRING,               // e.g. "any string"       // TODO: unicode (and escape) value evaluation
    TOKEN_ADD,                  // +
    TOKEN_SUB,                  // -
    TOKEN_MUL,                  // *
    TOKEN_DIV,                  // /
    TOKEN_NIL_COALESCING,       // ??
    TOKEN_EXCL_MARK,            // !
    TOKEN_NOT_EQUAL,            // !=
    TOKEN_EQUAL,                // ==
    TOKEN_ASSIGN,               // =
    TOKEN_GREATER,              // >
    TOKEN_GREATER_EQUAL,        // >=
    TOKEN_LESSER,               // <
    TOKEN_LESSER_EQUAL,         // <=
    TOKEN_L_BRACKET,            // (
    TOKEN_R_BRACKET,            // )
    TOKEN_L_CRLY_BRACKET,       // {
    TOKEN_R_CRLY_BRACKET,       // }
    TOKEN_COLON,                // :
    TOKEN_COMA,                 // ,
    TOKEN_ARROW,                // ->
    TOKEN_NEW_LINE,
    TOKEN_END_OF_FILE
} TokenType;

typedef struct {
    TokenType type;
    int line_index;

    union {                    // TODO: malloc error check
        char* string;          // TODO: lex errors
        int int_value;         // TODO: free(token)
        double float_value;
    } value;
} TokenData;

#endif /* TOKEN_H */