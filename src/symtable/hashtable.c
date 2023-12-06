/**
 * @name Implementation of IFJ23 language compiler
 * @file hashtable.c
 * @brief  contains functions for working with hash table
 *
 * @author Veronika Čalkovská (xcalko00)
 */

#include "hashtable.h"

#include "ListST/ListST.h"
#include "../helpers/parameters/parameters.h"
#include <string.h>
#include <stdio.h>

unsigned int _get_hash(char* str) {
    long long p = 29791, m = 1e9 + 7;
    unsigned hash = 0;
    long long p_pow = 1;
    for (int i = 0; str[i] != '\0' ; i++) {
        hash = (hash + (str[i] - 'a' + 1) * p_pow) % m;
        p_pow = (p_pow * p) % m;
    }
    return hash;
}

HTError _realloc(HTable* table){
    int cap = table->capacity;
    table->capacity = table->capacity * 2;

    LSTElement** new = ht_new(table->capacity);
    if(new == NULL)
        return H_ALLOC;

    unsigned hash;
    int index, step;
    int count = 0;

    for(int i = 0; i < cap; i ++){
        if(table->table[i] != NULL){

            if(count >= table->capacity){
                free(new);
                table->capacity = table->capacity * 2;
                new = ht_new(table->capacity);
                if(new == NULL)
                    return H_ALLOC;
                count = 0;
                i = 0;
            }
            
            hash = _get_hash((table->table[i])->identifier);
            index = hash % (table->capacity);
            step = hash % (table->capacity - 1) + 1;

            while(new[index] != NULL){
                if(index + step >= table->capacity){
                    index = index + step - table->capacity;
                }
                else{
                    index += step;
                }
            }
            new[index] = table->table[i]; 
             
            count++;   
        }
    }
   
    free(table->table);
    table->table = new;

    return H_OK;
}

LSTElement** ht_new(int size) {
    LSTElement** hashtable = malloc(size * sizeof(LSTElement*));
    if (hashtable == NULL) {
        return NULL;
    }
    for (int i = 0; i < size; i++) {
        hashtable[i] = NULL;
    }
    return hashtable;
}

void ht_rehash(LSTElement** hash_table, LSTElement** new, int size) {
    unsigned hash;
    int index, step;

    for (int i = 0; i < size; i++) {
        if (hash_table[i] != NULL) {
            hash = _get_hash(hash_table[i]->identifier);
            index = hash % size;
            step = hash % (size - 1) + 1;

            while (new[index] != NULL) {
                if ((index + step) >= size) {
                    index = index + step - size;
                } else
                    index += step;
            }
            new[index] = hash_table[i];
        }
    }
}

void ht_free(HTable* table) {
    for(int i = 0; i < table->capacity; i++){
        if(table->table[i] != NULL){
            if(table->table[i]->variant == FUNCTION){
                destroy_parameter_array(&(table->table[i]->value.parameters));  
            } 

            free(table->table[i]->identifier);   
            // TODO: needed?
            // if(list->firstItem->data->local_table[i]->return_type ==
            // T_STRING ||
            // list->firstItem->data->local_table[i]->return_type ==
            // T_STRING_NIL) {
            //     free(list->firstItem->data->local_table[i]->value.string_value);
            // }   

            free(table->table[i]); 
        }
    }

    free(table->table);
}

LSTElement* ht_search(HTable* table, char* identifier){
    unsigned int hash = _get_hash(identifier);
    int index = hash % table->capacity;
    int step = hash % (table->capacity - 1) + 1;

    int count = 0;

    while(table->table[index] != NULL && count < table->capacity){
        if(strcmp(table->table[index]->identifier, identifier) == 0){
            return table->table[index];
        }
        if(index + step >= table->capacity){
            index = index + step - table->capacity;
        } else {
            index += step;
        }
        count++;
    }

    return NULL;
}

HTError ht_insert(HTable* table, LSTElement* element){
    unsigned int hash = _get_hash(element->identifier);
    int index = hash % table->capacity;
    int step = hash % (table->capacity - 1) + 1;

    int count = 0;

    while(table->table[index] != NULL){
        
        if(count >= table->capacity){
            //LSTElement** new = ht_new(*size);
            if (_realloc(table) != H_OK) {
                return H_ALLOC;
            }
            hash = _get_hash(element->identifier);
            index = hash % table->capacity;
            step = hash % (table->capacity - 1) + 1;
            count = 0;       
        }
        if(index + step >= table->capacity){
            index = index + step - table->capacity;
        } else {
            index += step;
        }
        count++;
    }
    table->table[index] = element;
    //  printf("%d\n\n", index);
    return H_OK;
}

HTError ht_remove(HTable* table, char* identifier){
    unsigned int hash = _get_hash(identifier);
    int index = hash % table->capacity;
    int step = hash % (table->capacity - 1) + 1;

    int count = 0;

    while(table->table[index] != NULL && count < table->capacity){
        if(strcmp(table->table[index]->identifier, identifier) == 0){
            break;
        }
        else{
            if(index + step >= table->capacity){
                index = index + step - table->capacity;
            } else {
                index += step;
            }
        }
        count++;
    }

    if(table->table[index] == NULL)
        return H_SEARCH;
    
    if(table->table[index]->variant == FUNCTION){
        if (table->table[index]->value.parameters.parameters_arr != NULL && table->table[index]->defined_value == true) {
            free(table->table[index]->value.parameters.parameters_arr);
        }
    }
   
    free(table->table[index]);
    table->table[index] = NULL;

    LSTElement** new = ht_new(table->capacity);
    if (new == NULL) {
        return H_ALLOC;
    }
    
    ht_rehash(table->table, new, table->capacity);
    table->table = new;
    table->size -= 1;
    
    return H_OK;
}
