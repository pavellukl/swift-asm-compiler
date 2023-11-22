#include "parameters.h"

void init_parameter_array(Parameters *params) {
    params->infinite = false;
    params->capacity = 0;
    params->size = 0;
    params->parameters_arr = NULL;
}

bool add_to_parameter_array(Parameters *params, Parameter param) {
    if (params->capacity == params->size) {
        Parameter *buffer = realloc(params->parameters_arr,
                                    (params->capacity + PARAM_ARR_INC_N_ITEMS) *
                                        sizeof(struct Parameter));
        if (buffer == NULL) return false;

        params->parameters_arr = buffer;

        params->capacity += PARAM_ARR_INC_N_ITEMS;
    }

    params->parameters_arr[params->size] = param;
    params->size++;

    return true;
}

void invalidate_parameter_array(Parameters *params) { params->size = 0; }

void destroy_parameter_array(Parameters *params) {
    for (int i = 0; i < params->size; i++) {
        free(params->parameters_arr[i].identifier);
        free(params->parameters_arr[i].name);
    }
    free(params->parameters_arr);
}