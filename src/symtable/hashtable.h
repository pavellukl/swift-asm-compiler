/**
 * @file hashtable.h
 * @brief TODO: file header
 *
 * @author Veronika Čalkovská (xcalko00)
 */

#ifndef HASHTABLE_H
#define HASHTABLE_H

#include "ListST/ListST.h"



#define HT_SIZE 109


typedef enum { H_ALLOC, H_OK , H_SEARCH } HTError;

/**
 * Allocs new hashtable 
 * 
 * @param int   size of hashtable  
 * 
 * @return LSTElement**
*/
LSTElement** ht_new(int size);

/**
 * Frees hashtable 
 * 
 * @param LSTElement**  hashtable
 * @param int           size of hashtable
*/
void ht_free(HTable* table);

/**
 * Searches element in hashtable
 * 
 * @param LSTElement**  hashtable
 * @param char*         identifier of searched element
 * @param int           size of hashtable
 * 
 * @return LSTElement*  element or NULL if not found
*/
LSTElement* ht_search(HTable* table, char* identifier);

/**
 * Inserts new element to hastable
 * 
 * @param LSTElement**  hashtable
 * @param LSTElement*   element to be inserted
 * @param int           size of hashtable
 * 
 * @return HTError      result of the operation
*/
HTError ht_insert(HTable* table, LSTElement* element);

/**
 * Removes element from hashtable
 * 
 * @param LSTElement**  hashtable
 * @param char*         identifier of element to be removed
 * @param int           size of hashtable
 * 
 * @return HTError      result of the operation 
*/
HTError ht_remove(HTable* table, char* identifier);

//INSIDE FUNCTIONS
void ht_rehash(LSTElement** table, LSTElement** new_hashtable, int size);

unsigned int _get_hash(char* str);

HTError _realloc(HTable* table);

#endif