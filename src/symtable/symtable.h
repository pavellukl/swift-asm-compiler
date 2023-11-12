#ifndef SYMTABLE_H
#define SYMTABLE_H

#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include "../helpers/list/ListST/ListST.h"

#define HASH_SIZE 109

typedef enum{E_ALLOC, E_OK}Error;

/**
 * Creates list
 * 
 * @return ListST*  new list
*/
ListST* st_create_list(void);

/**
 * Destroys list
 * 
 * @param ListST*   pointer to list
*/
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
 * @return enum          result of the operation    
*/
STError st_add_element(ListST* list,  char* identifier, Type return_type, Variant variant, Value* value);

/**
 * Searches element in all scopes
 * 
 * @param ListST*       pointer to list
 * @param char*         identifier of searched element
 * 
 * @return LSTElement*  pointer to the element
*/
LSTElement* st_search_element(ListST* list, char* identifier);

/**
 * Removes first element with the same identifier in all scopes
 * 
 * @param ListST*       pointer to list
 * @param char*         identifier of searched element
 * 
 * @return STError      result of the operation
*/
STError st_remove_element(ListST* list, char* identifier);

/**
 * Updates value of the element
 * 
 * @param ListST*       pointer to list
 * @param char*         identifier of searched element
 * @param Value*        pointer to the new value
 * 
 * @return STError      result of the operation
*/
STError st_update_element(ListST* list, char* identifier, Value* value);

/**
 * Adds new scope to the list
 * 
 * @param ListST*   pointer to the list
 * @param char*     identifier of the scope
*/
STError st_push_scope(ListST* list, char* identifier);

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
STError st_remove_var(ListST* list, char* identifier);

/**
 * Removes function declaration in active scope
*/
STError st_remove_func(ListST* list, char* identifier);

/**
 * Searches element in active scope
 * 
 * @param ListST*   pointer to list
 * @param char*     identifier of searched element
*/
LSTElement* st_search_var(ListST* list, char* identifier);

/**
 * Searches function declaration in active scope
 * 
 * @param ListST*       pointer to list
 * @param char*         identifier of searched element
 * 
 * @return LSTElement*  pointer to the element
*/
LSTElement* st_search_func(ListST* list, char* identifier);

#endif /* SYMTABLE_H */
