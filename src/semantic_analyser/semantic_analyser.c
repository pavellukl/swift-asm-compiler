#include "semantic_analyser.h"

void analyze_function_dec(ParserOptions *parser_opt) {
    parser_opt = parser_opt;
    return;
}

void init_parameter_array(Parameters *params) {
    params->infinite = false;
    params->capacity = 0;
    params->size = 0;
    params->parameters_arr = NULL;
}

bool add_to_parameter_array(Parameters *params, Parameter new_param) {
    if (params->capacity == params->size) {
        Parameter *buffer = realloc(params->parameters_arr,
                                    (params->capacity + PARAM_ARR_INC_N_ITEMS) *
                                        sizeof(struct Parameter));
        if (buffer == NULL) {
            free(params->parameters_arr);
            init_parameter_array(params);
            return false;
        }

        params->parameters_arr = buffer;

        params->capacity += PARAM_ARR_INC_N_ITEMS;
    }

    params->parameters_arr[params->size] = new_param;
    params->size++;

    return true;
}

void invalidate_parameter_array(Parameters *params) { params->size = 0; }