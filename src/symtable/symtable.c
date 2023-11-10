#include "symtable_func.h"

unsigned int _get_hash(char* str){
	long long p = 29791, m = 1e9 + 7;
	unsigned hash = 0;
	long long p_pow = 1;
	for(int i = 0; i < (int)strlen(str); i++){
		hash = (hash + (str[i] - 'a' + 1) * p_pow) % m;
		p_pow = (p_pow * p) % m;
	}
	return hash;
}

int _new_global_scope(ListST* list, char* identifier){
    ListElement* new = malloc(sizeof(ListElement));
    if(new == NULL)
        return false;

    LSTElement** hash_table = malloc(HASH_SIZE*sizeof(LSTElement*));
    if(hash_table == NULL){
        free(new);
        return E_ALLOC;
    } 
 
   for(int i = 0; i < HASH_SIZE; i++){
        hash_table[i] = NULL;
    }

    new->identifier = identifier;
    new->local_table = hash_table;

    list_st_insert_first(list, new);

    list->activeItem = list->firstItem;

    return E_OK;
}

ListST* st_create_list(void){ 
    ListST* list = malloc(sizeof(ListST));
    if(list == NULL)
        return NULL;
    
    list_st_init(list);
    _new_global_scope(list, "global");

    return list;
}

int st_push_scope(ListST* list, char* identifier){
    ListElement* new = malloc(sizeof(ListElement));
    if(new == NULL)
        return E_ALLOC;
    
    LSTElement** hash_table = malloc(HASH_SIZE*sizeof(LSTElement*));
    if(hash_table == NULL){
        free(new);
        return E_ALLOC;
    }

    for(int i = 0; i < HASH_SIZE; i++){
        hash_table[i] = NULL;
    }

    new->identifier = identifier;
    new->local_table = hash_table;
   
    if(list_st_insert_before(list, new) != LIST_OK){
        free(new->local_table);
        free(new);
        return E_ALLOC;
    }

    list->activeItem = list->firstItem;

    return E_OK;
}

void _st_insert_element(ListItemST* scope, LSTElement* element){
    unsigned int hash = _get_hash(element->identifier);
    int index = hash % HASH_SIZE;
    int step = hash % (HASH_SIZE - 1) + 1;

    while(scope->data->local_table[index] != NULL){
        if((index + step) >= HASH_SIZE){
			index = index + step - HASH_SIZE;
		}
		else
			index += step;
    }
     
    scope->data->local_table[index] = element;
}

LSTElement* _create_element(char* identifier, Type return_type, Variant variant, Value* value){
    LSTElement* new = malloc(sizeof(LSTElement));
    if(new == NULL)
        return NULL;

    new->identifier = identifier;
    new->return_type = return_type;
    new->variant = variant;
    if(value == NULL){
        new->defined_value = false;
    }
    else{
        new->value = *value;
        new->defined_value = true;
    }

    return new;
}

int st_global_add_element(ListST* list,  char* identifier, Type return_type, Variant variant, Value* value){

    LSTElement* new = _create_element(identifier, return_type, variant, value);

    if(new == NULL)
        return E_ALLOC;

    ListItemST* tmp = list->firstItem;
    while(tmp->nextItem != NULL)
        tmp = tmp->nextItem;
    

    _st_insert_element(tmp, new);

    return E_OK;
}

int st_add_element(ListST* list,  char* identifier, Type return_type, Variant variant, Value* value){
    LSTElement* new = _create_element(identifier, return_type, variant, value);

    if(new == NULL)
        return E_ALLOC;

    _st_insert_element(list->firstItem, new);

    return E_OK;
}

LSTElement* st_search(ListST* list, char* identifier){
    unsigned hash = _get_hash(identifier);
    int index = hash % HASH_SIZE;
    int step = hash % (HASH_SIZE - 1) + 1;

    while(list->activeItem->data->local_table[index] != NULL){
        if(strcmp(list->activeItem->data->local_table[index]->identifier, identifier) == 0)
            return list->activeItem->data->local_table[index];

        if((index + step) >= HASH_SIZE){
			index = index + step - HASH_SIZE;
		}
		else
			index += step;
    } 

    return NULL;
}

LSTElement* st_search_global_func(ListST* list, char* identifier){
    unsigned hash = _get_hash(identifier);
    int index = hash % HASH_SIZE;
    int step = hash % (HASH_SIZE - 1) + 1; 

    ListItemST* tmp = list->firstItem;

    while(tmp->nextItem != NULL)
        tmp = tmp->nextItem;
    

    while(tmp->data->local_table[index] != NULL){
        if((strcmp(tmp->data->local_table[index]->identifier, identifier) == 0) && (tmp->data->local_table[index]->variant == FUNCTION)){
            return tmp->data->local_table[index];
        }

        if((index + step) >= HASH_SIZE){
			index = index + step - HASH_SIZE;
		}
		else
			index += step;
    } 

    return NULL;    
}

LSTElement* st_search_func(ListST* list, char* identifier){
   unsigned hash = _get_hash(identifier);
    int index = hash % HASH_SIZE;
    int step = hash % (HASH_SIZE - 1) + 1; 

    while(list->activeItem->data->local_table[index] != NULL){
        if((strcmp(list->activeItem->data->local_table[index]->identifier, identifier) == 0) && (list->activeItem->data->local_table[index]->variant == FUNCTION)){
            return list->activeItem->data->local_table[index];
        }

        if((index + step) >= HASH_SIZE){
			index = index + step - HASH_SIZE;
		}
		else
			index += step;
    } 

    return NULL;   
}

LSTElement* st_search_global_var(ListST* list, char* identifier){
   unsigned hash = _get_hash(identifier);
    int index = hash % HASH_SIZE;
    int step = hash % (HASH_SIZE - 1) + 1; 

    ListItemST* tmp = list->firstItem;

    while(tmp->nextItem != NULL)
        tmp = tmp->nextItem;
    

    while(tmp->data->local_table[index] != NULL){
        if((strcmp(tmp->data->local_table[index]->identifier, identifier) == 0) && (tmp->data->local_table[index]->variant != FUNCTION)){
            return tmp->data->local_table[index];
        }

        if((index + step) >= HASH_SIZE){
			index = index + step - HASH_SIZE;
		}
		else
			index += step;
    } 

    return NULL;  
}

void st_pop_scope(ListST* list){
    for(int i = 0; i < HASH_SIZE; i++){
        if(list->firstItem->data->local_table[i] != NULL){
            if(list->firstItem->data->local_table[i]->variant == FUNCTION){
                if(list->firstItem->data->local_table[i]->value.parameters.parameters_arr != NULL)
                    free(list->firstItem->data->local_table[i]->value.parameters.parameters_arr);
            }
            free(list->firstItem->data->local_table[i]);
        }
    }
    free(list->firstItem->data->local_table);
    free(list->firstItem->data);

    if(list->activeItem == list->firstItem)
        list_st_next(list);

    list_st_delete_first(list);
}

void _rehash(LSTElement** hash_table, LSTElement** new){
    unsigned hash;
    int index, step;

    for(int i = 0; i < HASH_SIZE; i++){
        if(hash_table[i] != NULL){
            hash = _get_hash(hash_table[i]->identifier);
            index = hash % HASH_SIZE;
            step = hash % (HASH_SIZE - 1) + 1; 

            while(new[index] != NULL){
                if((index + step) >= HASH_SIZE){
			        index = index + step - HASH_SIZE;
		        }
		        else
		        	index += step;
            }    
            new[index] = hash_table[i]; 
        }
    }
    free(hash_table);
}

int st_remove_var(ListST* list, char* identifier){
    unsigned hash = _get_hash(identifier);
    int index = hash % HASH_SIZE;
    int step = hash % (HASH_SIZE - 1) + 1;

    while((list->activeItem->data->local_table[index] != NULL)){
        if((strcmp(list->activeItem->data->local_table[index]->identifier, identifier) == 0) && (list->activeItem->data->local_table[index]->variant != FUNCTION)){
            break;
        }else{
            if((index + step) >= HASH_SIZE){
			    index = index + step - HASH_SIZE;
		    }
		    else
			    index += step;
        }
    }  

    if(list->activeItem->data->local_table[index] == NULL || list->activeItem->data->local_table[index]->variant == FUNCTION)
        return E_OK;

    free(list->activeItem->data->local_table[index]);
    list->activeItem->data->local_table[index] = NULL;

    LSTElement** new = malloc(HASH_SIZE*sizeof(LSTElement*));
    if(new == NULL)
        return E_ALLOC;

    _rehash(list->activeItem->data->local_table, new);

    list->activeItem->data->local_table = new;

    return E_OK;
}

int st_remove_global_var(ListST* list, char* identifier){
    unsigned hash = _get_hash(identifier);
    int index = hash % HASH_SIZE;
    int step = hash % (HASH_SIZE - 1) + 1;

    ListItemST* tmp = list->firstItem;
    while(tmp->nextItem != NULL)
        tmp = tmp->nextItem;

    while((tmp->data->local_table[index] != NULL)){
        if((strcmp(tmp->data->local_table[index]->identifier, identifier) == 0) && (tmp->data->local_table[index]->variant != FUNCTION)){
            break;
        }else{
            if((index + step) >= HASH_SIZE){
			    index = index + step - HASH_SIZE;
		    }
		    else
			    index += step;
        }
    }  

    if(tmp->data->local_table[index] == NULL || tmp->data->local_table[index]->variant == FUNCTION)
        return E_OK;

    free(tmp->data->local_table[index]);
    tmp->data->local_table[index] = NULL;

    LSTElement** new = malloc(HASH_SIZE*sizeof(LSTElement*));
    if(new == NULL)
        return E_ALLOC;

    _rehash(tmp->data->local_table, new);

    tmp->data->local_table = new;

    return E_OK;
}

int st_remove_global_func(ListST* list, char* identifier){
   unsigned hash = _get_hash(identifier);
    int index = hash % HASH_SIZE;
    int step = hash % (HASH_SIZE - 1) + 1;

    ListItemST* tmp = list->firstItem;

    while(tmp->nextItem != NULL)
        tmp = tmp->nextItem;

    while((tmp->data->local_table[index] != NULL)){
        if((strcmp(tmp->data->local_table[index]->identifier, identifier) == 0) && (tmp->data->local_table[index]->variant == FUNCTION)){
            break;
        }else{
            if((index + step) >= HASH_SIZE){
			    index = index + step - HASH_SIZE;
		    }
		    else
			    index += step;
        }
    } 

    if(tmp->data->local_table[index] == NULL)
        return E_OK;

    free(tmp->data->local_table[index]);
    tmp->data->local_table[index] = NULL;

    LSTElement** new = malloc(HASH_SIZE*sizeof(LSTElement*));
    if(new == NULL)
        return E_ALLOC;

    _rehash(tmp->data->local_table, new);
    tmp->data->local_table = new;

    return E_OK;
}

int st_remove_func(ListST* list, char* identifier){
   unsigned hash = _get_hash(identifier);
    int index = hash % HASH_SIZE;
    int step = hash % (HASH_SIZE - 1) + 1;

    while((list->activeItem->data->local_table[index] != NULL)){
        if((strcmp(list->activeItem->data->local_table[index]->identifier, identifier) == 0) && (list->activeItem->data->local_table[index]->variant == FUNCTION)){
            break;
        }else{
            if((index + step) >= HASH_SIZE){
			    index = index + step - HASH_SIZE;
		    }
		    else
			    index += step;
        }
    } 

    if(list->activeItem->data->local_table[index] == NULL)
        return E_OK;

    free(list->activeItem->data->local_table[index]);
    list->activeItem->data->local_table[index] = NULL;

    LSTElement** new = malloc(HASH_SIZE*sizeof(LSTElement*));
    if(new == NULL)
        return E_ALLOC;

    _rehash(list->activeItem->data->local_table, new);
    list->activeItem->data->local_table = new;

    return E_OK; 
}

void st_destroy_list(ListST* list){
    while(list->firstItem != NULL){
        st_pop_scope(list);
    }
    free(list);
    list = NULL;
}

