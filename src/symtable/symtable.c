#include "symtable.h"

unsigned int _get_hash(char* str) {
    long long p = 29791, m = 1e9 + 7;
    unsigned hash = 0;
    long long p_pow = 1;
    for (int i = 0; i < (int)strlen(str); i++) {
        hash = (hash + (str[i] - 'a' + 1) * p_pow) % m;
        p_pow = (p_pow * p) % m;
    }
    return hash;
}

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

    LSTElement** hash_table = malloc(HASH_SIZE * sizeof(LSTElement*));
    if (hash_table == NULL) {
        free(new);
        return E_ALLOC;
    }

    for (int i = 0; i < HASH_SIZE; i++) {
        hash_table[i] = NULL;
    }

    new->identifier = identifier;
    new->local_table = hash_table;
    new->max_size = HASH_SIZE;
    new->size = 0;

    if (list->firstItem == NULL) {
        if (list_st_insert_first(list, new) != LIST_OK) {
            free(new->local_table);
            free(new);
            return E_ALLOC;
        }
        list->activeItem = list->firstItem;
        return E_OK;
    }

    if (list_st_insert_before(list, new) != LIST_OK) {
        free(new->local_table);
        free(new);
        return E_ALLOC;
    }

    list->activeItem = list->firstItem;

    return E_OK;
}

STError _st_realloc(ListItemST* scope) {
    LSTElement** new = malloc(scope->data->max_size * 2 * sizeof(LSTElement*));
    if (new == NULL) return E_ALLOC;

    for (int i = 0; i < scope->data->max_size * 2; i++) {
        new[i] = NULL;
    }

    unsigned int hash;
    int index, step;

    for (int i = 0; i < scope->data->max_size; i++) {
        if (scope->data->local_table[i] != NULL) {
            hash = _get_hash(scope->data->local_table[i]->identifier);
            index = hash % (scope->data->max_size * 2);
            step = hash % ((scope->data->max_size * 2) - 1) + 1;

            while (new[index] != NULL) {
                if ((index + step) >= (scope->data->max_size) * 2) {
                    index = index + step - ((scope->data->max_size) * 2);
                } else
                    index += step;
            }
            new[index] = scope->data->local_table[i];
            // printf("%d %d %s\n", i, index, new[index]->identifier);
        }
    }

    free(scope->data->local_table);
    scope->data->local_table = new;
    scope->data->max_size = scope->data->max_size * 2;

    return E_OK;
}

void _st_insert_element(ListItemST* scope, LSTElement* element) {
    unsigned int hash = _get_hash(element->identifier);
    int index = hash % scope->data->max_size;
    int step = hash % (scope->data->max_size - 1) + 1;

    int count = 0;

    while (scope->data->local_table[index] != NULL) {
        if (count >= scope->data->max_size) {
            _st_realloc(scope);
            hash = _get_hash(element->identifier);
            index = hash % scope->data->max_size;
            step = hash % (scope->data->max_size - 1) + 1;
            count = 0;
        }
        if ((index + step) >= scope->data->max_size) {
            index = index + step - scope->data->max_size;
        } else
            index += step;
        count++;
    }

    scope->data->local_table[index] = element;
}

LSTElement* _create_element(char* identifier, Type return_type, Variant variant,
                            LSTElementValue* value) {
    LSTElement* new = malloc(sizeof(LSTElement));
    if (new == NULL) return NULL;

    new->identifier = identifier;
    new->return_type = return_type;
    new->variant = variant;
    if (value == NULL) {
        new->defined_value = false;
    } else {
        new->value = *value;
        new->defined_value = true;
    }

    return new;
}

STError st_add_element(ListST* list, char* identifier, Type return_type,
                       Variant variant, LSTElementValue* value) {
    if (list == NULL) return E_LIST;
    if (list->firstItem == NULL) return E_LIST;

    LSTElement* new = _create_element(identifier, return_type, variant, value);

    if (new == NULL) return E_ALLOC;

    if (list->firstItem->data->size >= (list->firstItem->data->max_size - 1)) {
        if (_st_realloc(list->firstItem) != E_OK) return E_ALLOC;
    }

    _st_insert_element(list->firstItem, new);
    list->firstItem->data->size += 1;

    return E_OK;
}

// TODO remove
LSTElement* st_search_var(ListST* list, char* identifier) {
    if (list == NULL) return NULL;
    if (list->firstItem == NULL) return NULL;

    unsigned hash = _get_hash(identifier);
    int index = hash % list->activeItem->data->max_size;
    int step = hash % (list->activeItem->data->max_size - 1) + 1;

    int count = 0;

    while (list->activeItem->data->local_table[index] != NULL) {
        if (count >= list->activeItem->data->max_size) return NULL;
        if ((strcmp(list->activeItem->data->local_table[index]->identifier,
                    identifier) == 0) &&
            (list->activeItem->data->local_table[index]->variant != FUNCTION))
            return list->activeItem->data->local_table[index];

        if ((index + step) >= list->activeItem->data->max_size) {
            index = index + step - list->activeItem->data->max_size;
        } else
            index += step;

        count++;
    }

    return NULL;
}

// TODO remove
LSTElement* st_search_func(ListST* list, char* identifier) {
    if (list == NULL) return NULL;
    if (list->firstItem == NULL) return NULL;

    ListItemST* tmp = list->firstItem;

    unsigned hash = _get_hash(identifier);
    int index, step;

    int count = 0;
    while (tmp != NULL) {
        index = hash % tmp->data->max_size;
        step = hash % (tmp->data->max_size - 1) + 1;

        while ((tmp->data->local_table[index] != NULL) &&
               (count < tmp->data->max_size)) {
            if ((strcmp(tmp->data->local_table[index]->identifier,
                        identifier) == 0) &&
                (tmp->data->local_table[index]->variant == FUNCTION)) {
                return tmp->data->local_table[index];
            }

            if ((index + step) >= tmp->data->max_size) {
                index = index + step - tmp->data->max_size;
            } else
                index += step;

            count++;
        }
        tmp = tmp->nextItem;
        count = 0;
    }
    return NULL;
}

void st_pop_scope(ListST* list) {
    if (list == NULL) return;
    if (list->firstItem == NULL) return;

    for (int i = 0; i < list->firstItem->data->max_size; i++) {
        if (list->firstItem->data->local_table[i] != NULL) {
            if (list->firstItem->data->local_table[i]->variant == FUNCTION) {
                if (list->firstItem->data->local_table[i]
                        ->value.parameters.parameters_arr != NULL)
                    free(list->firstItem->data->local_table[i]
                             ->value.parameters.parameters_arr);
            }
            free(list->firstItem->data->local_table[i]);
        }
    }

    // TODO: free strings (identifier and possibly string value if item is of
    // type string)

    free(list->firstItem->data->local_table);
    free(list->firstItem->data);

    if (list->activeItem == list->firstItem) list_st_next(list);

    list_st_delete_first(list);
}

void _rehash(LSTElement** hash_table, LSTElement** new, int size) {
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
    free(hash_table);
}

// TODO remove
STError st_remove_var(ListST* list, char* identifier) {
    if (list == NULL) return E_LIST;
    if (list->firstItem == NULL) return E_LIST;

    unsigned hash = _get_hash(identifier);
    int index = hash % list->activeItem->data->max_size;
    int step = hash % (list->activeItem->data->max_size - 1) + 1;

    if (!list_st_is_active(list)) return E_LIST;

    if (list->activeItem->data->local_table == NULL) return E_SEARCH;

    int count = 0;

    while ((list->activeItem->data->local_table[index] != NULL)) {
        if (count >= list->activeItem->data->max_size) return E_SEARCH;
        if ((strcmp(list->activeItem->data->local_table[index]->identifier,
                    identifier) == 0) &&
            (list->activeItem->data->local_table[index]->variant != FUNCTION)) {
            break;
        } else {
            if ((index + step) >= list->activeItem->data->max_size) {
                index = index + step - list->activeItem->data->max_size;
            } else
                index += step;
        }
        count++;
    }

    if (list->activeItem->data->local_table[index] == NULL ||
        list->activeItem->data->local_table[index]->variant == FUNCTION)
        return E_OK;

    free(list->activeItem->data->local_table[index]);
    list->activeItem->data->local_table[index] = NULL;

    LSTElement** new =
        malloc(list->activeItem->data->max_size * sizeof(LSTElement*));
    if (new == NULL) return E_ALLOC;

    for (int i = 0; i < list->activeItem->data->max_size; i++) {
        new[i] = NULL;
    }

    _rehash(list->activeItem->data->local_table, new,
            list->activeItem->data->max_size);

    list->activeItem->data->local_table = new;

    return E_OK;
}

STError st_remove_element(ListST* list, char* identifier) {
    if (list == NULL) return E_LIST;
    if (list->firstItem == NULL) return E_LIST;

    ListItemST* tmp = list->firstItem;

    unsigned hash = _get_hash(identifier);
    int index;
    int step;

    int count = 0;

    while (tmp != NULL) {
        index = hash % tmp->data->max_size;
        step = hash % (tmp->data->max_size - 1) + 1;
        while ((tmp->data->local_table[index] != NULL) &&
               (count < tmp->data->max_size)) {
            if ((strcmp(tmp->data->local_table[index]->identifier,
                        identifier) == 0)) {
                break;
            } else {
                if ((index + step) >= tmp->data->max_size) {
                    index = index + step - tmp->data->max_size;
                } else
                    index += step;
            }
            count++;
        }

        if ((tmp->data->local_table[index] != NULL) &&
            (strcmp(tmp->data->local_table[index]->identifier, identifier) ==
             0)) {
            break;
        }

        tmp = tmp->nextItem;
        count = 0;
    }

    if (tmp == NULL) return E_SEARCH;

    free(tmp->data->local_table[index]);
    tmp->data->local_table[index] = NULL;

    LSTElement** new = malloc(tmp->data->max_size * sizeof(LSTElement*));
    if (new == NULL) return E_ALLOC;

    for (int i = 0; i < tmp->data->max_size; i++) {
        new[i] = NULL;
    }

    _rehash(tmp->data->local_table, new, tmp->data->max_size);
    tmp->data->local_table = new;

    return E_OK;
}

STError st_update_element(ListST* list, char* identifier,
                          LSTElementValue* value) {
    if (list == NULL) return E_LIST;
    if (list->firstItem == NULL) return E_LIST;

    LSTElement* element = st_search_element(list, identifier, NULL);
    if (element == NULL) return E_SEARCH;

    if (element->variant == FUNCTION || element->variant == CONSTANT)
        return E_SEARCH;

    element->value = *value;
    if (value != NULL) element->defined_value = true;

    return E_OK;
}

// TODO remove
STError st_remove_func(ListST* list, char* identifier) {
    if (list == NULL) return E_LIST;
    if (list->firstItem == NULL) return E_LIST;

    unsigned hash = _get_hash(identifier);
    int index = hash % list->activeItem->data->max_size;
    int step = hash % (list->activeItem->data->max_size - 1) + 1;

    if (!list_st_is_active(list)) return E_LIST;

    int count = 0;

    while ((list->activeItem->data->local_table[index] != NULL)) {
        if (count >= list->activeItem->data->max_size) return E_SEARCH;
        if ((strcmp(list->activeItem->data->local_table[index]->identifier,
                    identifier) == 0) &&
            (list->activeItem->data->local_table[index]->variant == FUNCTION)) {
            break;
        } else {
            if ((index + step) >= list->activeItem->data->max_size) {
                index = index + step - list->activeItem->data->max_size;
            } else
                index += step;
        }
        count++;
    }

    if (list->activeItem->data->local_table[index] == NULL) return E_SEARCH;

    free(list->activeItem->data->local_table[index]);
    list->activeItem->data->local_table[index] = NULL;

    LSTElement** new =
        malloc(list->activeItem->data->max_size * sizeof(LSTElement*));
    if (new == NULL) return E_ALLOC;

    for (int i = 0; i < list->activeItem->data->max_size; i++) {
        new[i] = NULL;
    }

    _rehash(list->activeItem->data->local_table, new,
            list->activeItem->data->max_size);
    list->activeItem->data->local_table = new;

    return E_OK;
}

LSTElement* st_search_element(ListST* list, char* identifier,
                              int* scope_identifier) {
    if (list == NULL) return NULL;
    if (list->firstItem == NULL) return NULL;

    ListItemST* tmp = list->firstItem;

    unsigned hash = _get_hash(identifier);
    int index;
    int step;

    int count = 0;
    while (tmp != NULL) {
        index = hash % tmp->data->max_size;
        step = hash % (tmp->data->max_size - 1) + 1;
        while ((tmp->data->local_table[index] != NULL) &&
               (count < tmp->data->max_size)) {
            if ((strcmp(tmp->data->local_table[index]->identifier,
                        identifier) == 0)) {
                if (scope_identifier != NULL) {
                    *scope_identifier = tmp->data->identifier;
                }
                return tmp->data->local_table[index];
            } else {
                if ((index + step) >= tmp->data->max_size) {
                    index = index + step - tmp->data->max_size;
                } else
                    index += step;
            }
            count++;
        }
        tmp = tmp->nextItem;
        count = 0;
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

STError st_push_func_scope(ListST* list, LSTElement* element, int identifier) {
    if (list == NULL) return E_LIST;

    if (list->firstItem == NULL) return E_LIST;

    if (st_push_scope(list, identifier) != E_OK) return E_ALLOC;

    if (element->value.parameters.parameters_arr != NULL) {
        for (int i = 0; i < element->value.parameters.size; i++) {
            // if parameter identifier is '_', we can discard it
            if (!strcmp(element->value.parameters.parameters_arr[i].identifier,
                        "_")) {
                continue;
            }

            if (st_add_element(
                    list,
                    element->value.parameters.parameters_arr[i].identifier,
                    element->value.parameters.parameters_arr[i].par_type,
                    CONSTANT, NULL) != E_OK)
                return E_ALLOC;
        }
    }

    return E_OK;
}
