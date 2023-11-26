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

typedef struct Argument {
    char* name;
    char* identifier;
    Type par_type;
    TokenValue value;
    TokenType token_type;
} Argument;

typedef struct Arguments {
    int size;
    int capacity;
    Argument* argument_arr;
} Arguments;

typedef struct Parameter {
    char* name;
    char* identifier;
    Type par_type;
} Parameter;

typedef struct Parameters {
    bool infinite;
    int size;
    int capacity;
    Parameter* parameters_arr;
} Parameters;

typedef union {
    long long int int_value;
    char* string_value;
    double float_value;
    bool bool_value;
    Parameters parameters;
} LSTElementValue;

typedef struct LSTElement {
    char* identifier;
    Type return_type;
    Variant variant;
    bool defined_value;
    LSTElementValue value;
} LSTElement;

typedef struct ListElement {
    int identifier;
    LSTElement** local_table;  // hash table
    int size;
    int max_size;
} ListElement;

LIST_DEC(ListElement*, ST, st)

#endif /* LIST_ST_H */
