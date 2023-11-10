#ifndef SYMTABLE_H
#define SYMTABLE_H

#include "../helpers/list/list.h"

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
}Value;

typedef struct LSTElement{
    char* identifier;
    Type return_type;
    Variant variant;
    bool defined_value;
    Value value;
}LSTElement;


typedef struct ListElement{
    char* identifier;
    LSTElement** local_table;    //hash table
}ListElement;

LIST_DEC(ListElement*, ST, st)

/**
 * Creates list
 * 
 * @return ListST*  new list
*/
ListST* st_create_list(void);

void st_destroy_list(ListST* list);

/**
 * Adds element to LST of active scope
 * 
 * @param ListST*       pointer to list
 * @param char*         identifier of the element
 * @param Type          type of the element/return type of the function
 * @param Variant       variant of the element
 * @param Value*        pointer to the value of the element of the function
 * 
 * @return int          result of the operation    
*/
int st_add_element(ListST* list,  char* identifier, Type return_type, Variant variant, Value* value);

/**
 * Searches element in active scope
 * 
 * @param ListST*   pointer to list
 * @param char*     identifier of searched element
*/
LSTElement* st_search(ListST* list, char* identifier);

/**
 * Searches function declaration in active scope
*/
LSTElement* st_search_func(ListST* list, char* identifier);

/**
 * Searches function declaration in global scope
 * 
 * @param ListST*   pointer to list
 * @param char*     identifier of searched element
*/
LSTElement* st_search_global_func(ListST* list, char* identifier);

/**
 * Searches variable in global scope
 * 
 * @param ListST*   pointer to the list
 * @param char*     identifier of searched element 
*/
LSTElement* st_search_global_var(ListST* list, char* identifier);

/**
 * Adds new scope to the list
 * 
 * @param ListST*   pointer to the list
 * @param char*     identifier of the scope
*/
int st_push_scope(ListST* list, char* identifier);

/**
 * Pops actual (first) scope
 * 
 * @param ListST*   pointer to the list
*/
void st_pop_scope(ListST* list);

/**
 * Removes variable in active scope 
 * 
 * @param ListST*   pointer to the list
 * @param char*     identifier
*/
int st_remove_var(ListST* list, char* identifier);

/**
 * Removes function declaration in active scope
*/
int st_remove_func(ListST* list, char* identifier);

/**
 * Removes function declaration in global scope
 * 
 * @param ListST*   pointer to the list
 * @param char*     identifier of the function
*/
int st_remove_global_func(ListST* list, char* identifier);

int st_remove_global_var(ListST* list, char* identifier);


#endif
