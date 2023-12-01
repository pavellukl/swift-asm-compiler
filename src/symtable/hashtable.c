/**
 * @file hashtable.c
 * @brief TODO: file header
 *
 * @author Veronika Čalkovská (xcalko00)
 */

#include "hashtable.h"
#include "../helpers/parameters/parameters.h"
#include <string.h>

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

HTError _realloc(LSTElement** hashtable, int* size){
    *size = *size * 2;

    LSTElement** new = ht_new(*size);
    if (new == NULL) {
        return H_ALLOC;
    }

    ht_rehash(hashtable, new, *size);
    hashtable = new;

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

void ht_free(LSTElement** hashtable, int size) {
    for(int i = 0; i < size; i++){
        if(hashtable[i] != NULL){
            if(hashtable[i]->variant == FUNCTION){
                destroy_parameter_array(&(hashtable[i]->value.parameters));  
            } 

            free(hashtable[i]->identifier);   
            // TODO: needed?
            // if(list->firstItem->data->local_table[i]->return_type ==
            // T_STRING ||
            // list->firstItem->data->local_table[i]->return_type ==
            // T_STRING_NIL) {
            //     free(list->firstItem->data->local_table[i]->value.string_value);
            // }   

            free(hashtable[i]); 
        }
    }
 
    free(hashtable);
    *hashtable = NULL;
}

LSTElement* ht_search(LSTElement** hashtable, char* identifier, int size){
    unsigned int hash = _get_hash(identifier);
    int index = hash % size;
    int step = hash % (size - 1) + 1;

    int count = 0;

    while(hashtable[index] != NULL && count < size){
        if(strcmp(hashtable[index]->identifier, identifier) == 0){
            return hashtable[index];
        }
        if(index + step >= size){
            index = index + step - size;
        } else {
            index += step;
        }
        count++;
    }

    return NULL;
}

HTError ht_insert(LSTElement** hashtable, LSTElement* element, int size){
    unsigned int hash = _get_hash(element->identifier);
    int index = hash % size;
    int step = hash % (size - 1) + 1;

    int count = 0;

    while(hashtable[index] != NULL){
        if(count >= size){
            if (_realloc(hashtable, &size) == H_ALLOC) {
                return H_ALLOC;
            }
            hash = _get_hash(element->identifier);
            index = hash % size;
            step = hash % (size - 1) + 1;
            count = 0;       
        }
        if(index + step >= size){
            index = index + step - size;
        } else {
            index += step;
        }
        count++;
    }
    hashtable[index] = element;
    return H_OK;
}

HTError ht_remove(LSTElement** hashtable, char* identifier, int size){
    unsigned int hash = _get_hash(identifier);
    int index = hash % size;
    int step = hash % (size - 1) + 1;

    int count = 0;

    while(hashtable[index] != NULL && count < size){
        if(strcmp(hashtable[index]->identifier, identifier) == 0){
            break;
        }
        else{
            if(index + step >= size){
                index = index + step - size;
            } else {
                index += step;
            }
        }
        count++;
    }

    if(hashtable[index] == NULL)
        return H_SEARCH;
    
    if(hashtable[index]->variant == FUNCTION){
        if (hashtable[index]->value.parameters.parameters_arr != NULL && hashtable[index]->defined_value == true) {
            free(hashtable[index]->value.parameters.parameters_arr);
        }
    }
   
    free(hashtable[index]);
    hashtable[index] = NULL;

    LSTElement** new = ht_new(size);
    if (new == NULL) {
        return H_ALLOC;
    }
    
    ht_rehash(hashtable, new, size);
    hashtable = new;
    
    return H_OK;
}
