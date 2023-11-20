#include "semantic_analyser.h"

bool analyze_function_dec(ParserOptions *parser_opt) {
    Parameters params = parser_opt->variables.identif.value.parameters;

    // check if names and identifiers are different for all parameters
    for (int i = 0; i < params.size; i++) {
        Parameter param = params.parameters_arr[i];

        if (!strcmp(param.name, param.identifier)) {
            parser_opt->return_code = OTHER_ERR;
            return false;
        }
    }

    return true;
}

bool analyze_function_call(ParserOptions *parser_opt, char *identifier,
                           Parameters *arguments) {
    // get called function
    LSTElement *func = st_search_func(parser_opt->symtable, identifier);

    // if function doesn't exist
    if (func == NULL) {
        parser_opt->return_code = DEF_ERR;
        return false;
    }

    // TODO rest: check args and params
    arguments = arguments;
    return false;
}

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
        if (buffer == NULL) {
            free(params->parameters_arr);
            init_parameter_array(params);
            return false;
        }

        params->parameters_arr = buffer;

        params->capacity += PARAM_ARR_INC_N_ITEMS;
    }

    params->parameters_arr[params->size] = param;
    params->size++;

    return true;
}

void invalidate_parameter_array(Parameters *params) { params->size = 0; }

void destroy_parameter_array(Parameters *params) {
    free(params->parameters_arr);
    init_parameter_array(params);
}

bool is_simple_expression(PPListItemType pp_type) {
    return pp_type == TERMINAL_IDENTIF || pp_type == TERMINAL_INT ||
           pp_type == TERMINAL_FLOAT || pp_type == TERMINAL_STRING ||
           pp_type == TERMINAL_BOOL || pp_type == TERMINAL_KEYWORD_NIL;
}

bool is_binary_operator(PPListItemType pp_type) {
    return pp_type == TERMINAL_ADD || pp_type == TERMINAL_SUB ||
           pp_type == TERMINAL_MUL || pp_type == TERMINAL_DIV ||
           pp_type == TERMINAL_EQUAL || pp_type == TERMINAL_NOT_EQUAL ||
           pp_type == TERMINAL_LESSER || pp_type == TERMINAL_LESSER_EQUAL ||
           pp_type == TERMINAL_GREATER || pp_type == TERMINAL_GREATER_EQUAL ||
           pp_type == TERMINAL_AND || pp_type == TERMINAL_OR ||
           pp_type == TERMINAL_NIL_COALESCING;
}

Type _remove_nilable(Type type) {
    switch (type) {
        case T_INT_NIL:
            return T_INT;
        case T_FLOAT_NIL:
            return T_FLOAT;
        case T_STRING_NIL:
            return T_STRING;
        case T_BOOL_NIL:
            return T_BOOL;
        default:
            return type;
    }
}

bool _is_number_type(Type type) {
    return type == T_INT || type == T_FLOAT || type == T_INT_NIL ||
           type == T_FLOAT_NIL;
}

bool _is_nilable_type(Type type) {
    return type == T_INT_NIL || type == T_FLOAT_NIL || type == T_STRING_NIL ||
           type == T_BOOL_NIL;
}

bool analyze_binary_operation(ParserOptions *parser_opt,
                              TokenType operator_type, Type l_op_type,
                              Type r_op_type, Type *new_data_type) {
    // E -> E OPERATOR E
    // data type tests and new data type resolving
    switch (operator_type) {
        case TOKEN_ADD:
            // both should be only int or float or string (not nilable)
            if ((l_op_type != T_STRING || r_op_type != T_STRING) &&
                (!_is_number_type(l_op_type) ||
                 !_is_number_type(r_op_type || _is_nilable_type(l_op_type) ||
                                  _is_nilable_type(r_op_type)))) {
                parser_opt->return_code = EXPRTYPE_ERR;
                return false;
            }
            if (l_op_type == T_FLOAT || r_op_type == T_FLOAT) {
                *new_data_type = T_FLOAT;
            } else {
                *new_data_type = l_op_type;
            }
            break;
        case TOKEN_SUB:
        case TOKEN_MUL:
        case TOKEN_DIV:
            // both should be only int or float (not nilable)
            if (!_is_number_type(l_op_type) ||
                !_is_number_type(r_op_type || _is_nilable_type(l_op_type) ||
                                 _is_nilable_type(r_op_type))) {
                parser_opt->return_code = EXPRTYPE_ERR;
                return false;
            }
            if (l_op_type == T_FLOAT || r_op_type == T_FLOAT) {
                *new_data_type = T_FLOAT;
            } else {
                *new_data_type = l_op_type;
            }
            break;
        case TOKEN_EQUAL:
        case TOKEN_NOT_EQUAL:
            // both should be of the same type (int and float are same)
            // operands can't be nilable type (nil as well)
            if ((l_op_type != r_op_type) &&
                (!_is_number_type(l_op_type) || !_is_number_type(r_op_type))) {
                parser_opt->return_code = EXPRTYPE_ERR;
                return false;
            }
            if (_is_nilable_type(l_op_type) || _is_nilable_type(r_op_type) ||
                l_op_type == T_NIL) {
                parser_opt->return_code = EXPRTYPE_ERR;
                return false;
            }
            *new_data_type = T_BOOL;
            break;
        case TOKEN_LESSER:
        case TOKEN_LESSER_EQUAL:
        case TOKEN_GREATER:
        case TOKEN_GREATER_EQUAL:
            // both should be of the same type (int and float are same)
            // operands can't be nilable type (not nil as well)
            // operands can't be bool
            if ((l_op_type != r_op_type) &&
                (!_is_number_type(l_op_type) || !_is_number_type(r_op_type))) {
                parser_opt->return_code = EXPRTYPE_ERR;
                return false;
            }
            if (_is_nilable_type(l_op_type) || _is_nilable_type(r_op_type) ||
                l_op_type == T_BOOL || l_op_type == T_NIL) {
                parser_opt->return_code = EXPRTYPE_ERR;
                return false;
            }
            *new_data_type = T_BOOL;
            break;
        case TOKEN_AND:
        case TOKEN_OR:
            // operands must be bool
            // operands can't be nilable type (not nil as well)
            if (l_op_type != T_BOOL || r_op_type != T_BOOL) {
                parser_opt->return_code = EXPRTYPE_ERR;
                return false;
            }
            *new_data_type = T_BOOL;
            break;
        case TOKEN_NIL_COALESCING:
            /* right operand must be of the same type as left operand
               (but without nilable part) */
            // operands can't be nil
            if (l_op_type == T_NIL || l_op_type != _remove_nilable(r_op_type)) {
                parser_opt->return_code = EXPRTYPE_ERR;
                return false;
            }
            *new_data_type = r_op_type;
            break;
        default:
            parser_opt->return_code = STX_ERR;
            return false;
    }
    return true;
}