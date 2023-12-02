/**
 * @file semantic_analyser.c
 * @brief Contains the implementation of the functions for the semantic
 *        analysis.
 * 
 * @author Pavel Lukl (xluklp00)
 * @author Jan Klanica (xklani00)
 */

#include "semantic_analyser.h"

void init_semantic_context(SemanticContext *sem_ctx) {
    sem_ctx->current_fnc = NULL;
    sem_ctx->has_function_all_returns = false;
    sem_ctx->n_scope_commands = 0;
    sem_ctx->is_eol_before_required = false;
}

bool analyze_function_dec(ParserOptions *parser_opt, Parameters *params) {
    for (int i = 0; i < params->size; i++) {
        Parameter param = params->parameters_arr[i];

        // check if names and identifiers are different
        if (param.name != NULL && param.identifier != NULL &&
            !strcmp(param.name, param.identifier)) {
            parser_opt->return_code = OTHER_ERR;
            return false;
        }

        // check if identifiers are unique
        if (param.identifier == NULL) continue;

        for (int j = i + 1; j < params->size; j++) {
            Parameter tmp = params->parameters_arr[j];

            if (tmp.identifier == NULL) continue;

            if (!strcmp(param.identifier, tmp.identifier)) {
                parser_opt->return_code = OTHER_ERR;
                return false;
            }
        }
    }

    return true;
}

bool analyze_generate_function_call(ParserOptions *parser_opt, char *identifier,
                                    Arguments *arguments, Type *return_type) {
    //  get called function
    LSTElement *func =
        st_search_element(parser_opt->symtable, identifier, NULL);

    // if function doesn't exist or identifier is not of a function
    if (func == NULL || func->variant != FUNCTION) {
        parser_opt->return_code = DEF_ERR;
        return false;
    }

    // if number of arguments doesn not match number of function parameters
    if (arguments->size != func->value.parameters.size &&
        !func->value.parameters.infinite) {
        parser_opt->return_code = FNCALL_ERR;
        return false;
    }

    // if parameters are infinite
    if (func->value.parameters.infinite) {
        // push arguments in reverse when function has infinite parameters
        for (int i = arguments->size - 1; i >= 0; i--) {
            Argument call_arg = arguments->argument_arr[i];

            // if argument has a name
            if (call_arg.name != NULL) {
                parser_opt->return_code = FNCALL_ERR;
                return false;
            }

            // if argument is not a literal -> it is an identifier
            if (call_arg.identifier != NULL) {
                // check argument identifier
                LSTElement *el = st_search_element(parser_opt->symtable,
                                                   call_arg.identifier, NULL);

                // if identifier is not defined or has no defined value
                if (el == NULL || !el->defined_value) {
                    parser_opt->return_code = UNDEFVAR_ERR;
                    return false;
                }

                // if identifier is not a constant or a variable
                if (el->variant != CONSTANT && el->variant != VARIABLE) {
                    parser_opt->return_code = DEF_ERR;
                    return false;
                }
            }

            if (!generate_argument(parser_opt->gen_var, parser_opt->symtable,
                                   call_arg, T_VOID)) {
                parser_opt->return_code = INTER_ERR;
                return false;
            };
        }

        // last parameter for infinite function is the number of arguments
        if (!generate_argument(parser_opt->gen_var, parser_opt->symtable,
                               (Argument){.identifier = NULL,
                                          .name = NULL,
                                          .par_type = T_INT,
                                          .token_type = TOKEN_INT,
                                          .value.int_value = arguments->size},
                               T_VOID)) {
            parser_opt->return_code = INTER_ERR;
            return false;
        };

        if (!generate_fnc_call(parser_opt->gen_var, identifier)) {
            parser_opt->return_code = INTER_ERR;
            return false;
        }

        return true;
    }

    // if parameters are not infinite

    // check validity of arguments against function parameters
    for (int i = 0; i < arguments->size; i++) {
        Parameter func_param = func->value.parameters.parameters_arr[i];
        Argument *call_arg = &arguments->argument_arr[i];

        // if parameter and argument names do not match
        if ((func_param.name != NULL || call_arg->name != NULL) &&
            (func_param.name == NULL || call_arg->name == NULL ||
             strcmp(func_param.name, call_arg->name) != 0)) {
            parser_opt->return_code = FNCALL_ERR;
            return false;
        }

        // if argument is not a literal -> it is an identifier
        if (call_arg->identifier != NULL) {
            // check argument identifier
            LSTElement *el = st_search_element(parser_opt->symtable,
                                               call_arg->identifier, NULL);

            // if identifier is not defined or has no defined value
            if (el == NULL || !el->defined_value) {
                parser_opt->return_code = UNDEFVAR_ERR;
                return false;
            }

            // if identifier is not a constant or a variable
            if (el->variant != CONSTANT && el->variant != VARIABLE) {
                parser_opt->return_code = DEF_ERR;
                return false;
            }

            // save element type for type check
            call_arg->par_type = el->return_type;
        }
        // if argument is literal
        else {
            // retype literal to float if argument is int and expected type is
            // float
            if ((func_param.par_type == T_FLOAT ||
                 func_param.par_type == T_FLOAT_NIL) &&
                call_arg->par_type == T_INT) {
                call_arg->par_type = T_FLOAT;
            }
        }

        // if types do not match
        if (!_do_types_match(func_param.par_type, call_arg->par_type)) {
            parser_opt->return_code = FNCALL_ERR;
            return false;
        }

        if (!generate_argument(parser_opt->gen_var, parser_opt->symtable,
                               *call_arg, T_VOID)) {
            parser_opt->return_code = INTER_ERR;
            return false;
        };
    }

    // save function return type
    if (return_type != NULL) *return_type = func->return_type;

    if (!generate_fnc_call(parser_opt->gen_var, identifier)) {
        parser_opt->return_code = INTER_ERR;
        return false;
    }

    return true;
}

bool analyze_generate_assignment(ParserOptions *parser_opt, char *identifier,
                                 ASTNode *expression_node, bool is_function) {
    // get variable that's being assigned to
    LSTElement *el = st_search_element(parser_opt->symtable, identifier, NULL);

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

    // retype expression to float if expression is int and expected type is
    // float
    if ((el->return_type == T_FLOAT || el->return_type == T_FLOAT_NIL) &&
        expression_node->data_type == T_INT && !is_function) {
        expression_node->data_type = T_FLOAT;
    }

    // if types don't match
    if (!_do_types_match(el->return_type, expression_node->data_type)) {
        parser_opt->return_code = EXPRTYPE_ERR;
        return false;
    }

    el->defined_value = true;

    // pop the return value of the function or expression from the stack to
    // the desired variable
    if (!generate_assignment(parser_opt->gen_var, parser_opt->symtable,
                             identifier)) {
        parser_opt->return_code = INTER_ERR;
        return false;
    }

    return true;
}

bool analyze_generate_return(ParserOptions *parser_opt, LSTElement *fnc,
                             ASTNode *expression_node) {
    // return can not be in the global scope
    if (st_is_global_active(parser_opt->symtable)) {
        //! semantic check returning syntax error
        parser_opt->return_code = STX_ERR;
        return false;
    }

    // retype expression to float if expression is int and expected type is
    // float
    if ((fnc->return_type == T_FLOAT || fnc->return_type == T_FLOAT_NIL) &&
        expression_node->data_type == T_INT &&
        expression_node->token.type != TOKEN_IDENTIF) {
        expression_node->data_type = T_FLOAT;
    }

    // if return without expression in non void fnc
    if (fnc->return_type != T_VOID && expression_node->data_type == T_VOID) {
        parser_opt->return_code = FNRET_ERR;
        return false;
    }

    // if return with expression in void fnc
    if (fnc->return_type == T_VOID && expression_node->data_type != T_VOID) {
        parser_opt->return_code = FNRET_ERR;
        return false;
    }

    // if function type and return expression type do not match
    if (!_do_types_match(fnc->return_type, expression_node->data_type)) {
        parser_opt->return_code = FNCALL_ERR;
        return false;
    }

    if (fnc->return_type != T_VOID) {
        if (!generate_expression(&parser_opt->gen_var, expression_node,
                                parser_opt->symtable)) {
            parser_opt->return_code = INTER_ERR;
            return false;
        }
    }

    if (!generate_function_end(parser_opt->gen_var.scope)) {
        parser_opt->return_code = INTER_ERR;
        return false;
    }

    return true;
}

bool analyze_var_def(ParserOptions *parser_opt, bool is_constant,
                     char *identifier, Type expected_type,
                     ASTNode *provided_value_node, bool is_function) {
    int found_scope_id = -1;
    // try to find variable with the same identifier
    LSTElement *el =
        st_search_element(parser_opt->symtable, identifier, &found_scope_id);

    // check for same scope redeclaration
    if ((el != NULL && found_scope_id == parser_opt->gen_var.scope_n)) {
        parser_opt->return_code = DEF_ERR;
        return false;
    }

    // retype expression to float if expression is int and expected type is
    // float
    if ((expected_type == T_FLOAT || expected_type == T_FLOAT_NIL) &&
        provided_value_node->data_type == T_INT && !is_function) {
        provided_value_node->data_type = T_FLOAT;
    }

    // if type isn't explicitly provided and no value is provided
    if ((provided_value_node->data_type == T_VOID ||
         provided_value_node->data_type == T_NIL) &&
        is_function) {
        parser_opt->return_code = EXPRTYPE_ERR;
        return false;
    } else if (expected_type == T_VOID &&
               (provided_value_node->data_type == T_VOID ||
                provided_value_node->data_type == T_NIL)) {
        parser_opt->return_code = UNDEFTYPE_ERR;
        return false;
    }

    // if expected and provided types do not match
    if (expected_type != T_VOID && provided_value_node->data_type != T_VOID &&
        !_do_types_match(expected_type, provided_value_node->data_type)) {
        parser_opt->return_code = EXPRTYPE_ERR;
        return false;
    }

    bool is_value_defined = provided_value_node->data_type != T_VOID ||
                            _is_nilable_type(expected_type);
    LSTElementValue var_val = {0};

    // actual variable type to be saved in symtable
    Type actual_type = expected_type != T_VOID ? expected_type
                                               : provided_value_node->data_type;

    // add defined variable to symtable
    STError err = st_add_element(parser_opt->symtable, identifier, actual_type,
                                 is_constant ? CONSTANT : VARIABLE, var_val,
                                 is_value_defined);

    if (err != E_OK) {
        parser_opt->return_code = INTER_ERR;
        return false;
    }
    return true;
}

bool analyze_if_let(ParserOptions *parser_opt, char *identifier,
                    Type *initial_type, LSTElement **found_var) {
    // try to find specified variable
    LSTElement *el = st_search_element(parser_opt->symtable, identifier, NULL);

    // if variable does not exist
    if (el == NULL) {
        parser_opt->return_code = UNDEFVAR_ERR;
        return false;
    }

    // save found variable
    *found_var = el;
    // save initial variable type
    *initial_type = el->return_type;
    // remove nil from type for if branch
    el->return_type = _remove_nilable(el->return_type);

    return true;
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

bool _do_types_match(Type l_type, Type r_type) {
    if (l_type == r_type) return true;

    switch (l_type) {
        case T_BOOL_NIL:
            if (r_type == T_BOOL || r_type == T_NIL) return true;
            break;
        case T_FLOAT_NIL:
            if (r_type == T_FLOAT || r_type == T_NIL) return true;
            break;
        case T_INT_NIL:
            if (r_type == T_INT || r_type == T_NIL) return true;
            break;
        case T_STRING_NIL:
            if (r_type == T_STRING || r_type == T_NIL) return true;
            break;
        default:
            break;
    }

    return false;
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
            if (l_op_type == T_NIL || _remove_nilable(l_op_type) != r_op_type) {
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