/**
 * @name Implementation of IFJ23 language compiler
 * @file symtable.c
 * @brief TODO: file header
 *
 * @author Veronika Čalkovská (xcalko00)
 * @author Pave Lukl (xluklp00)
 */

#include "symtable.h"
#include "hashtable.h"


ListST* st_create_list(void) {
    ListST* list = malloc(sizeof(ListST));
    if (list == NULL) return NULL;

    list_st_init(list);

    return list;
}

STError st_push_scope(ListST* list, int identifier) {
    if (list == NULL) return E_LIST;

    ListElement* new = malloc(sizeof(ListElement));
    if (new == NULL) return E_ALLOC;

    LSTElement** hash_table = ht_new(HASH_SIZE);

    new->identifier = identifier;
    new->table.table = hash_table;
    new->table.capacity = HASH_SIZE;
    new->table.size = 0;

    if (list->firstItem == NULL) {
        if (list_st_insert_first(list, new) != LIST_OK) {
            free(new->table.table);
            free(new);
            return E_ALLOC;
        }
        list->activeItem = list->firstItem;
        return E_OK;
    }

    if (list_st_insert_before(list, new) != LIST_OK) {
        free(new->table.table);
        free(new);
        return E_ALLOC;
    }

    list->activeItem = list->firstItem;

    return E_OK;
}

LSTElement* _create_element(char* identifier, Type return_type, Variant variant,
                            LSTElementValue value, bool defined_value) {
    LSTElement* new = malloc(sizeof(LSTElement));
    if (new == NULL) return NULL;

    new->identifier = identifier;
    new->return_type = return_type;
    new->variant = variant;
    new->value = value;
    new->defined_value = defined_value;

    return new;
}

STError st_add_element(ListST* list, char* identifier, Type return_type,
                       Variant variant, LSTElementValue value,
                       bool defined_value) {
    if (list == NULL) return E_LIST;
    if (list->firstItem == NULL) return E_LIST;

    LSTElement* new =
        _create_element(identifier, return_type, variant, value, defined_value);

    if (new == NULL) return E_ALLOC;

    if (list->firstItem->data->table.size >= (list->firstItem->data->table.capacity - 1)) {
        //LSTElement** newa = ht_new(list->firstItem->data->max_size*2);
        if ( _realloc(&list->firstItem->data->table) != H_OK)
            return E_ALLOC;
        // free(list->firstItem->data->local_table);
        // list->activeItem->data->local_table = newa;
        //   LSTElement** newa = ht_new(list->firstItem->data->max_size);
        //      if (newa == NULL) {
        //          return E_ALLOC;
        //      } 

        // unsigned hash;
        // int index, step;

        //     for(int i = 0; i < list->firstItem->data->max_size; i ++){
        //           if(list->activeItem->data->local_table[i] != NULL){
            
        //          hash = _get_hash(list->firstItem->data->local_table[i]->identifier);
        //          index = hash % (list->firstItem->data->max_size*2);
        //          step = hash % (list->firstItem->data->max_size*2 - 1) + 1;

        //             while(newa[index] != NULL){
        //                if(index * step >= list->firstItem->data->max_size * 2){
        //                  index = index + step - list->firstItem->data->max_size * 2;
        //               }
        //               else{
        //                  index += step;
        //               }
        //          }
        //         newa[index] =list->firstItem->data->local_table[i]; 
             
        //     // printf("%d\n", index);     
        //      }
        //  }
        // free(list->firstItem->data->local_table);
        // list->firstItem->data->local_table = newa;
    }
    
    ht_insert(&(list->firstItem->data->table), new);
    list->firstItem->data->table.size += 1;

    // for(int i = 0; i < list->firstItem->data->max_size; i++){
    //     if(list->firstItem->data->local_table[i]!= NULL){
    //         printf("%d neni null\n", i);
    //     }
    // }
    // printf("\n------------\n");

    return E_OK;
}

void st_pop_scope(ListST* list) {
    if (list == NULL) return;
    if (list->firstItem == NULL) return;

    ht_free(&(list->firstItem->data->table));
    
    //free(list->firstItem->data->local_table);
    free(list->firstItem->data);

    if (list->activeItem == list->firstItem) list_st_next(list);

    list_st_delete_first(list);
}

STError st_remove_element(ListST* list, char* identifier) {
    if (list == NULL) return E_LIST;
    if (list->firstItem == NULL) return E_LIST;

    ListItemST* tmp = list->firstItem;

    HTError err;

    while (tmp != NULL) {

        err = ht_remove(&(tmp->data->table), identifier);
        if (err == H_OK) {
            break;
        }
        if (err == H_ALLOC) {
            return E_ALLOC;
        }

        tmp = tmp->nextItem;
    }

    if (tmp == NULL) return E_SEARCH;

    return E_OK;
}

STError st_update_element(ListST* list, char* identifier, LSTElementValue value,
                          bool defined_value) {
    if (list == NULL) return E_LIST;
    if (list->firstItem == NULL) return E_LIST;

    LSTElement* element = st_search_element(list,
                                    identifier, NULL);
    if (element == NULL) return E_SEARCH;

    if (element->variant == FUNCTION || element->variant == CONSTANT)
        return E_SEARCH;

    element->value = value;
    element->defined_value = defined_value;

    return E_OK;
}

LSTElement* st_search_element(ListST* list, char* identifier,
                              int* scope_identifier) {
    if (list == NULL) return NULL;
    if (list->firstItem == NULL) return NULL;

    ListItemST* tmp = list->firstItem;

    LSTElement* element = NULL;
   
    while (tmp != NULL) {

        element = ht_search(&(tmp->data->table), identifier);

        if(element != NULL) {
            if(scope_identifier != NULL) 
                *scope_identifier = tmp->data->identifier;
            return element;
        }

        tmp = tmp->nextItem;
    }

    return NULL;
}

void st_destroy_list(ListST* list) {
    if (list == NULL) return;
    while (list->firstItem != NULL) {
        st_pop_scope(list);
    }
    free(list);
    list = NULL;
}

bool st_is_global_active(ListST* list) {
    if (list == NULL) return false;

    if (list->firstItem == NULL) return false;

    if (list->firstItem->nextItem != NULL) return false;

    return true;
}

STError st_push_func_scope(ListST* list, LSTElement* element) {
    if (list == NULL) return E_LIST;

    if (list->firstItem == NULL) return E_LIST;

    if (st_push_scope(list, 1) != E_OK) return E_ALLOC;

    if (element->value.parameters.parameters_arr != NULL) {
        for (int i = 0; i < element->value.parameters.size; i++) {
            // if parameter identifier is '_' which is signified by NULL, we can
            // discard it
            if (element->value.parameters.parameters_arr[i].identifier ==
                NULL) {
                continue;
            }

            LSTElementValue val = {0};

            char* param_identifier;
            clone_string(
                &param_identifier,
                element->value.parameters.parameters_arr[i].identifier);

            if (st_add_element(
                    list, param_identifier,
                    element->value.parameters.parameters_arr[i].par_type,
                    CONSTANT, val, true) != E_OK)
                return E_ALLOC;
        }
    }

    return E_OK;
}
