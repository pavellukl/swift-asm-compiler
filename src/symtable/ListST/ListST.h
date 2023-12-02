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


/**
 * Return type of element in symtable
*/
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

/**
 * Variant of element in symtable
*/
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

/**
 * Parameter of function 
*/
typedef struct Parameter {
    char* name;
    char* identifier;
    Type par_type;
} Parameter;

/**
 * Parameters of function declared in symtable
 * 
 * @param bool      true - infinite num of params
 * @param int       number of params
 * @param int       capacity of array of parameters
*/
typedef struct Parameters {
    bool infinite;
    int size;
    int capacity;
    Parameter* parameters_arr;
} Parameters;

/**
 * Value of element in symtable
 * Depends on return type of element
*/
typedef union {
    long long int int_value;
    char* string_value;
    double float_value;
    bool bool_value;
    Parameters parameters;
} LSTElementValue;

/**
 * Element of hashtable in scope
 * 
 * @param char*             identifier
 * @param Type              return type
 * @param Variant           variant of element
 * @param bool              true - value is defined
 * @param LSTElementValue   value of element if defined 
*/
typedef struct LSTElement {
    char* identifier;
    Type return_type;
    Variant variant;
    bool defined_value;
    LSTElementValue value;
} LSTElement;

typedef struct hashtable{
    LSTElement** table;
    int size;
    int capacity;
}HTable;

/**
 * Element of stack of scopes
 * 
 * @param int           identifier
 * @param LSTElement**  local hashtable
*/
typedef struct ListElement {
    int identifier;
    HTable table; 
} ListElement;

LIST_DEC(ListElement*, ST, st)

#endif /* LIST_ST_H */
