/**
 * @name Implementation of IFJ23 language compiler
 * @file arguments.c
 * @brief Contains implementation of the functions operating upon Arguments
 * struct
 *
 * @author Pavel Lukl (xluklp00)
 */

#include "arguments.h"

void init_argument_array(Arguments *arguments) {
    arguments->capacity = 0;
    arguments->size = 0;
    arguments->argument_arr = NULL;
}

bool add_to_argument_array(Arguments *arguments, Argument argument) {
    if (arguments->capacity == arguments->size) {
        Argument *buffer =
            realloc(arguments->argument_arr,
                    (arguments->capacity + ARGS_ARR_INC_N_ITEMS) *
                        sizeof(struct Argument));
        if (buffer == NULL) return false;

        arguments->argument_arr = buffer;

        arguments->capacity += ARGS_ARR_INC_N_ITEMS;
    }

    arguments->argument_arr[arguments->size] = argument;
    arguments->size++;

    return true;
}

void invalidate_argument_array(Arguments *arguments) { arguments->size = 0; }

void destroy_argument_array(Arguments *arguments) {
    for (int i = 0; i < arguments->size; i++) {
        free(arguments->argument_arr[i].identifier);
        free(arguments->argument_arr[i].name);
        if (arguments->argument_arr[i].token_type == TOKEN_STRING) {
            free(arguments->argument_arr[i].value.string);
        }
    }
    free(arguments->argument_arr);
}