/**
 * @file scanner_types.h
 * @brief header file containing automat states and structures necessary for 
 * input file handling and scanner buffer handling
 * 
 * @author Denis Milistenfer (xmilis00)
 */

#ifndef SCANNER_TYPES_H
#define SCANNER_TYPES_H

#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "StackTokenData/StackTokenData.h"
#include "token.h"

// AutomatStates: all possible states that finite state machine can get to
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
    MULTILINE_STRING,
    MULTILINE_QUOTE_ONE,
    MULTILINE_QUOTE_TWO,
    STRING_END,
    ESCAPE_SEQUENCE,
    UNICODE_SEQUENCE,
    UNICODE_VALUE,
    QUESTION_MARK,
    NIL_COALESCING,
    OR,
    AND
} AutomatState;

// ScannerOptions: structure for handling input file, keeping track of current line,
// token id and contains StackTokenData stack where returned tokens from parser are stored
// in order to achieve "lookahead" effect 
typedef struct {
    char *file;
    int i;
    int size;
    int line_counter;
    int id_counter;
    StackTokenData *returned_tokens;
} ScannerOptions;

// ScannerBuffer: structure for handling scanner buffer
typedef struct {
    char *buffer;
    int i;
    int size;
} ScannerBuffer;

#endif /* SCANNER_TYPES_H */
