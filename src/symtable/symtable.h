/**
 * @file symtable.h
 * @brief TODO: file header
 * 
 * @author your name (you@domain.com)
 */

#ifndef SYMTABLE_H
#define SYMTABLE_H

#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "../helpers/parameters/parameters.h"
#include "../helpers/utils.h"
#include "ListST/ListST.h"

#define HASH_SIZE 109

typedef enum { E_ALLOC, E_SEARCH, E_LIST, E_OK } STError;

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
 * @param Value         value of the element
 * @param bool          bool whether the element has a defined value
 *
 * @return enum          result of the operation
 */
STError st_add_element(ListST* list, char* identifier, Type return_type,
                       Variant variant, LSTElementValue value,
                       bool defined_value);

/**
 * Searches element in all scopes
 *
 * @param ListST*       pointer to list
 * @param char*         identifier of searched element
 * @param int*          pointer to where the scope id of found element will be
 * stored
 *
 * @return LSTElement*  pointer to the element
 */
LSTElement* st_search_element(ListST* list, char* identifier,
                              int* scope_identifier);

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
 * @param Value         new value
 * @param bool          bool whether the element has a defined value

 *
 * @return STError      result of the operation
 */
STError st_update_element(ListST* list, char* identifier, LSTElementValue value,
                          bool defined_value);

/**
 * Adds new scope to the list
 *
 * @param ListST*   pointer to the list
 * @param int     identifier of the scope
 */
STError st_push_scope(ListST* list, int identifier);

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

/**
 * Checks if global scope is active scope
 *
 * @param ListST*   pointer to list
 *
 * @return bool     true if global scope is active scope
 */
bool st_is_global_active(ListST* list);

/**
 * Pushes new function scope and adds parameters of passed function to
 * the new scope
 *
 * @param ListST*       pointer to list
 * @param LSTElement*   pointer to function element
 *
 * @return STError      result of the operation
 */
STError st_push_func_scope(ListST* list, LSTElement* element);

#endif /* SYMTABLE_H */
