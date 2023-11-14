#ifndef SCANNER_H
#define SCANNER_H

#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "StackTokenData/StackTokenData.h"
#include "token.h"
#include "../debug.h"

typedef enum {
    START,
    DIV,
    LINE_COMMENT,
    BLOCK_COMMENT,
    BLOCK_COMMENT_END,
    ADD,
    SUB,
    ARROW,
    WHITE_SPACE,
    END_OF_FILE,
    MUL,
    EXCL_MARK,
    NOT_EQUAL,
    ASSIGN,
    EQUAL,
    GREATER,
    GREATER_EQUAL,
    LESSER,
    LESSER_EQUAL,
    L_BRACKET,
    R_BRACKET,
    L_CRLY_BRACKET,
    R_CRLY_BRACKET,
    COLON,
    COMA,
    UNDERSCORE,
    IDENTIF,
    NUM,
    DECIMAL_POINT,
    DECIMAL_VALUE,
    EXP_MARK,
    EXP_SIGN,
    EXPONENT,
    STRING,
    STRING_END,
    ESCAPE_SEQUENCE,
    UNICODE_SEQUENCE,
    UNICODE_VALUE,
    QUESTION_MARK,
    NIL_COALESCING,
    OR,
    AND
} AutomatState;

typedef struct {
    char *file;
    int i;
    int size;
    int line_counter;
    StackTokenData *returned_tokens;
} ScannerOptions;

TokenData is_keyword(char *keyword, bool new_line_before_token);

bool _skip_whitespaces(ScannerOptions *opt);

void scanner_opt_free(ScannerOptions *opt);

bool scanner_opt_init(ScannerOptions *opt, FILE *file);

void scanner_rewind_file(ScannerOptions *opt);

int get_next_char(ScannerOptions *opt);

TokenData get_next_token(ScannerOptions *opt);

#endif /* SCANNER_H */
