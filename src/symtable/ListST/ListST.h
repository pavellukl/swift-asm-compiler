#ifndef LIST_ST_H
#define LIST_ST_H

#include "../../helpers/list.h"

typedef enum { T_VOID, T_INT, T_FLOAT, T_STRING, T_BOOL } Type;

typedef enum { CONSTANT, VARIABLE, FUNCTION } Variant;

typedef struct Parameter {
    char* name;
    char* identifier;
    Type par_type;
} Parameter;

typedef struct Parameters {
    bool infinite;
    int size;
    Parameter* parameters_arr;
} Parameters;

typedef union {
    int int_value;
    char* string_value;
    float float_value;
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
    char* identifier;
    LSTElement** local_table;  // hash table
    int size;
    int max_size;
} ListElement;

LIST_DEC(ListElement*, ST, st)

#endif /* LIST_ST_H */
