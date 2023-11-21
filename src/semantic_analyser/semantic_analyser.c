#include "semantic_analyser.h"

bool analyze_function_dec(ParserOptions *parser_opt) {
    Parameters params = parser_opt->variables.identif.value.parameters;

    for (int i = 0; i < params.size; i++) {
        Parameter param = params.parameters_arr[i];

        // check if names and identifiers are different
        if (!strcmp(param.name, param.identifier)) {
            parser_opt->return_code = OTHER_ERR;
            return false;
        }

        // check if identifiers are unique
        if (!strcmp(param.identifier, "_")) continue;

        for (int j = i + 1; j < params.size; j++) {
            Parameter tmp = params.parameters_arr[i];

            if (!strcmp(param.identifier, tmp.identifier)) {
                parser_opt->return_code = OTHER_ERR;
                return false;
            }
        }
    }

    return true;
}

bool analyze_function_call(ParserOptions *parser_opt, char *identifier,
                           Parameters *arguments, Type *return_type) {
    // get called function
    LSTElement *func = st_search_element(parser_opt->symtable, identifier);

    // if function doesn't exist or identifier is not of a function
    if (func == NULL || func->variant != FUNCTION) {
        parser_opt->return_code = DEF_ERR;
        return false;
    }

    // if number of arguments doesn not match number of function parameters
    if (arguments->size != func->value.parameters.size) {
        parser_opt->return_code = FNCALL_ERR;
        return false;
    }

    // check validity of arguments against function parameters
    for (int i = 0; i < arguments->size; i++) {
        Parameter func_param = func->value.parameters.parameters_arr[i];
        Parameter call_arg = arguments->parameters_arr[i];

        // if parameter and argument names do not match
        if (strcmp(func_param.name, call_arg.name) != 0) {
            parser_opt->return_code = FNCALL_ERR;
            return false;
        }

        // if argument is not a literal -> it is an identifier
        if (strcmp(func_param.identifier, "") != 0) {
            // check argument identifier
            LSTElement *el =
                st_search_element(parser_opt->symtable, identifier);

            // if identifier is not defined or has no defined value
            if (el == NULL || !el->defined_value) {
                parser_opt->return_code = UNDEFVAR_ERR;
                return false;
            }

            // if identifier is not a constant or a variable
            if (el->variant != CONSTANT || el->variant != VARIABLE) {
                parser_opt->return_code = DEF_ERR;
                return false;
            }

            // save element type for type check
            call_arg.par_type = el->return_type;
        }

        // if types do not match
        if (func_param.par_type != call_arg.par_type &&
            (!_is_nilable_type(func_param.par_type) ||
             call_arg.par_type != T_NIL)) {
            parser_opt->return_code = FNCALL_ERR;
            return false;
        }
    }

    // save function return type
    *return_type = func->return_type;

    return true;
}

bool analyze_assignment(ParserOptions *parser_opt, char *identifier,
                        Type assign_type) {
    // get variable that's being assigned to
    LSTElement *el = st_search_element(parser_opt->symtable, identifier);

    // if variable doesn't exist
    if (el == NULL) {
        parser_opt->return_code = UNDEFVAR_ERR;
        return false;
    }

    // if trying to assign to constant or function
    if (el->variant == CONSTANT || el->variant == FUNCTION) {
        parser_opt->return_code = OTHER_ERR;
        return false;
    }

    // if types don't match
    if (assign_type != el->return_type &&
        (!_is_nilable_type(el->return_type) || assign_type != T_NIL)) {
        parser_opt->return_code = EXPRTYPE_ERR;
        return false;
    }

    el->defined_value = true;

    return true;
}

bool analyze_return(ParserOptions *parser_opt, LSTElement fnc,
                    Type expression_type) {
    // return can not be in the global scope
    if (st_is_global_active(parser_opt->symtable)) {
        //! semantic check returning syntax error
        parser_opt->return_code = STX_ERR;
        return false;
    }

    // if function type and return expression type do not match
    if (fnc.return_type != expression_type &&
        (!_is_nilable_type(fnc.return_type) || expression_type != T_NIL)) {
        parser_opt->return_code = FNRET_ERR;
        return false;
    }

    return true;
}

bool analyze_var_def(ParserOptions *parser_opt, bool is_constant,
                     char *identifier, Type expected_type,
                     Type provided_value_type) {
    // try to find variable with the same identifier
    LSTElement *el = st_search_element(parser_opt->symtable, identifier);

    // TODO: handle same scope redeclaration
    if ((el != NULL /*&& found_within_same_scope*/)) {
        parser_opt->return_code = DEF_ERR;
        return false;
    }

    // if type isn't explicitly provided and no value is provided
    if (expected_type == T_VOID &&
        (provided_value_type == T_VOID || provided_value_type == T_NIL)) {
        parser_opt->return_code = UNDEFTYPE_ERR;
        return false;
    }

    // if expected and provided types do not match
    if (expected_type != provided_value_type &&
        (!_is_nilable_type(expected_type) || provided_value_type != T_NIL)) {
        parser_opt->return_code = EXPRTYPE_ERR;
        return false;
    }

    bool is_value_defined = provided_value_type != T_VOID;
    LSTElementValue var_val = {0};

    // actual variable type to be saved in symtable
    Type actual_type =
        expected_type != T_VOID ? expected_type : provided_value_type;

    // add defined variable to symtable
    STError err = st_add_element(parser_opt->symtable, identifier, actual_type,
                                 is_constant ? CONSTANT : VARIABLE,
                                 is_value_defined ? &var_val : NULL);

    if (err != E_OK) return false;

    return true;
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
            if ((l_op_type != T_STRING || r_op_type != T_STRING) 
                &&
                (!_is_number_type(l_op_type) || !_is_number_type(r_op_type ||
                 _is_nilable_type(l_op_type) || _is_nilable_type(r_op_type)))) {
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
                !_is_number_type(r_op_type ||
                _is_nilable_type(l_op_type) ||
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