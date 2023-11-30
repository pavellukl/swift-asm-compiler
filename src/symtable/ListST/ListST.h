/**
 * @file ListST.h
 * @brief TODO: file header
 * 
 * @author your name (you@domain.com)
 */

#ifndef LIST_ST_H
#define LIST_ST_H

#include "../../helpers/list.h"
#include "../../scanner/token.h"

typedef enum {
    T_VOID,
    T_INT,
    T_INT_NIL,
    T_FLOAT,
    T_FLOAT_NIL,
    T_STRING,
    T_STRING_NIL,
    T_BOOL,
    T_BOOL_NIL,
    T_NIL
} Type;

typedef enum { CONSTANT, VARIABLE, FUNCTION } Variant;

// TODO: comments
typedef struct Argument {
    char* name;
    char* identifier;
    Type par_type;
    TokenValue value;
    TokenType token_type;
} Argument;

// TODO: comments
typedef struct Arguments {
    int size;
    int capacity;
    Argument* argument_arr;
} Arguments;

// TODO: comments
typedef struct Parameter {
    char* name;
    char* identifier;
    Type par_type;
} Parameter;

// TODO: comments
typedef struct Parameters {
    bool infinite;
    int size;
    int capacity;
    Parameter* parameters_arr;
} Parameters;

// TODO: comments
typedef union {
    long long int int_value;
    char* string_value;
    double float_value;
    bool bool_value;
    Parameters parameters;
} LSTElementValue;

// TODO: comments
typedef struct LSTElement {
    char* identifier;
    Type return_type;
    Variant variant;
    bool defined_value;
    LSTElementValue value;
} LSTElement;

// TODO: comments
typedef struct ListElement {
    int identifier;
    LSTElement** local_table;  // hash table
    int size;
    int max_size;
} ListElement;

LIST_DEC(ListElement*, ST, st)

#endif /* LIST_ST_H */
