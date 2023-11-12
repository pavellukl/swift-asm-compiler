#ifndef SYMTABLE_TYPES_H
#define SYMTABLE_TYPES_H

#include <stdbool.h>

#define HASH_SIZE 109

typedef enum{T_INT, T_FLOAT, T_STRING, T_BOOL}Type;

typedef enum{CONSTANT, VARIABLE, FUNCTION}Variant;

typedef enum{E_ALLOC, E_OK}Error;

typedef struct Parameter{
    char* name;
    char* identifier;
    Type par_type;
}Parameter;

typedef struct Parameters{
    bool infinite;
    int size;
    Parameter* parameters_arr;
}Parameters;

typedef union {
        int int_value;
        char* string_value;
        float float_value;
        bool bool_value;
        Parameters parameters;
}LSTElementValue;

typedef struct LSTElement{
    char* identifier;
    Type return_type;
    Variant variant;
    bool defined_value;
    LSTElementValue value;
}LSTElement;

typedef struct ListElement{
    char* identifier;
    LSTElement** local_table;    //hash table
}ListElement;

#endif /* SYMTABLE_TYPES_H */