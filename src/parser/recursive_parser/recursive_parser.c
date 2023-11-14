#include "recursive_parser.h"

bool _look_ahead_for_fn(ParserOptions *parser_opt, bool *is_function) {
    if (parser_opt->token.type != TOKEN_IDENTIF) {
        *is_function = false;
        return true;
    }

    LSTElement *el =
        st_search_element(parser_opt->symtable, parser_opt->token.value.string);

    if (el == NULL) {
        parser_opt->return_code = UNDEFVAR_ERR;
        return false;
    }

    if (el->variant == FUNCTION) {
        *is_function = true;
        return true;
    }

    *is_function = false;
    return true;
}

bool _program(ParserOptions *parser_opt) {
    if (parser_opt->token.type == TOKEN_END_OF_FILE) {
        _next_token(parser_opt);
        return true;
    } else if (parser_opt->token.type == TOKEN_KEYWORD_FUNC) {
        return _function_definition(parser_opt) && _program(parser_opt);
    } else if (parser_opt->token.type == TOKEN_KEYWORD_FUNC ||
               parser_opt->token.type == TOKEN_KEYWORD_VAR ||
               parser_opt->token.type == TOKEN_KEYWORD_LET ||
               parser_opt->token.type == TOKEN_KEYWORD_IF ||
               parser_opt->token.type == TOKEN_KEYWORD_WHILE) {
        return _command(parser_opt) && _program(parser_opt);
    }
    parser_opt->return_code = STX_ERR;
    return false;
}

bool _function_definition(ParserOptions *parser_opt) {
    //! in first run generate function
    //! in second run do not generate
    //! view 'parser_opt->is_first_run' for info about run
    if (parser_opt->token.type == TOKEN_KEYWORD_FUNC) {
        // initialize parameter array
        parser_opt->variables.new_identif.value.parameters.parameters_arr =
            malloc(PARAM_ARR_INITIAL_N_ITEMS * sizeof(struct Parameter));
        if (parser_opt->variables.new_identif.value.parameters.parameters_arr ==
            NULL) {
            parser_opt->return_code = INTER_ERR;
            return false;
        }

        parser_opt->variables.new_identif.value.parameters.size = 0;
        parser_opt->variables.new_identif.value.parameters.capacity =
            PARAM_ARR_INITIAL_N_ITEMS;
        parser_opt->variables.new_identif.value.parameters.infinite = false;

        if (_function_head(parser_opt) && _scope_body(parser_opt)) {
            // define function if identif is not in symtable and on first run
            if (parser_opt->is_first_run) {
                parser_opt->variables.new_identif.defined_value = true;
                parser_opt->variables.new_identif.variant = FUNCTION;

                // TODO check completed function semantically

                // add function to symtable
                STError res = st_add_element(
                    parser_opt->symtable,
                    parser_opt->variables.new_identif.identifier,
                    parser_opt->variables.new_identif.return_type,
                    parser_opt->variables.new_identif.variant,
                    &(parser_opt->variables.new_identif.value));

                if (res != E_OK) {
                    free(parser_opt->variables.new_identif.value.parameters
                             .parameters_arr);
                    parser_opt->return_code = INTER_ERR;
                    return false;
                }
            }

            // TODO generate function in target code

            // free helper parameter array
            free(parser_opt->variables.new_identif.value.parameters
                     .parameters_arr);

            return true;
        };

        // free helper parameter array
        free(parser_opt->variables.new_identif.value.parameters.parameters_arr);

        return false;
    }

    parser_opt->return_code = STX_ERR;
    return false;
}

bool _function_head(ParserOptions *parser_opt) {
    if (parser_opt->token.type != TOKEN_KEYWORD_FUNC) {
        parser_opt->return_code = STX_ERR;
        return false;
    }
    _next_token(parser_opt);
    if (parser_opt->token.type != TOKEN_IDENTIF) {
        parser_opt->return_code = STX_ERR;
        return false;
    }

    // check symtable for identif
    LSTElement *el =
        st_search_func(parser_opt->symtable, parser_opt->token.value.string);

    // if identif already defined on first run
    if (el != NULL && parser_opt->is_first_run) {
        parser_opt->return_code = DEF_ERR;
        return false;
    }

    // save new function name
    parser_opt->variables.new_identif.identifier =
        parser_opt->token.value.string;

    _next_token(parser_opt);
    if (parser_opt->token.type != TOKEN_L_BRACKET) {
        parser_opt->return_code = STX_ERR;
        return false;
    }
    _next_token(parser_opt);
    if (!_param_list(parser_opt)) {
        return false;
    }
    if (parser_opt->token.type != TOKEN_R_BRACKET) {
        parser_opt->return_code = STX_ERR;
        return false;
    }
    _next_token(parser_opt);

    return __func_identif_lbracket_arglist_rbracket(parser_opt);
}

bool __func_identif_lbracket_arglist_rbracket(ParserOptions *parser_opt) {
    if (parser_opt->token.type == TOKEN_L_CRLY_BRACKET) {
        // save function type as void when return type omitted
        parser_opt->variables.new_identif.return_type = T_VOID;
        return true;
    } else if (parser_opt->token.type == TOKEN_ARROW) {
        _next_token(parser_opt);

        // if data type grammar check passes
        if (_data_type(parser_opt)) {
            // save function type
            parser_opt->variables.new_identif.return_type =
                parser_opt->variables.new_type;
            return true;
        }

        // if data type grammar check fails
        return false;
    }
    parser_opt->return_code = STX_ERR;
    return false;
}

bool _param_list(ParserOptions *parser_opt) {
    if (parser_opt->token.type == TOKEN_R_BRACKET) {
        // save that function has no parameters
        parser_opt->variables.new_identif.value.parameters.size = 0;

        return true;
    } else if (parser_opt->token.type == TOKEN_IDENTIF ||
               parser_opt->token.type == TOKEN_UNDERSCORE) {
        return _param(parser_opt) && _comma_param(parser_opt);
    }
    parser_opt->return_code = STX_ERR;
    return false;
}

bool _comma_param(ParserOptions *parser_opt) {
    if (parser_opt->token.type == TOKEN_R_BRACKET) {
        return true;
    } else if (parser_opt->token.type == TOKEN_COMA) {
        _next_token(parser_opt);
        return _param(parser_opt) && _comma_param(parser_opt);
    }
    parser_opt->return_code = STX_ERR;
    return false;
}

bool _param(ParserOptions *parser_opt) {
    if (parser_opt->token.type == TOKEN_IDENTIF ||
        parser_opt->token.type == TOKEN_UNDERSCORE) {
        // save parameter name to new parameter
        switch (parser_opt->token.type) {
            case TOKEN_IDENTIF:
                parser_opt->variables.new_param.name = "_";
                break;
            case TOKEN_UNDERSCORE:
                parser_opt->variables.new_param.name =
                    parser_opt->token.value.string;
                break;
            default:
                break;
        }

        _next_token(parser_opt);
        return __param_name(parser_opt);
    }
    parser_opt->return_code = STX_ERR;
    return false;
}

bool __param_name(ParserOptions *parser_opt) {
    if (parser_opt->token.type == TOKEN_IDENTIF ||
        parser_opt->token.type == TOKEN_UNDERSCORE) {
        // save parameter identifier to new parameter
        switch (parser_opt->token.type) {
            case TOKEN_IDENTIF:
                parser_opt->variables.new_param.identifier = "_";
                break;
            case TOKEN_UNDERSCORE:
                parser_opt->variables.new_param.identifier =
                    parser_opt->token.value.string;
                break;
            default:
                break;
        }

        _next_token(parser_opt);
        if (parser_opt->token.type != TOKEN_COLON) {
            parser_opt->return_code = STX_ERR;
            return false;
        }
        _next_token(parser_opt);

        // if data type grammar check passes
        if (_data_type(parser_opt)) {
            // save function type
            parser_opt->variables.new_param.par_type =
                parser_opt->variables.new_type;

            // check if parameter array is out of capacity and reallocate
            if (parser_opt->variables.new_identif.value.parameters.size ==
                parser_opt->variables.new_identif.value.parameters.capacity) {
                Parameter *buffer = realloc(
                    parser_opt->variables.new_identif.value.parameters
                        .parameters_arr,
                    parser_opt->variables.new_identif.value.parameters.size *
                        2 * sizeof(struct Parameter));
                if (buffer == NULL) {
                    free(parser_opt->variables.new_identif.value.parameters
                             .parameters_arr);

                    parser_opt->return_code = INTER_ERR;
                    return false;
                }

                parser_opt->variables.new_identif.value.parameters
                    .parameters_arr = buffer;
                parser_opt->variables.new_identif.value.parameters.capacity *=
                    2;
            }

            // add new param to param array
            parser_opt->variables.new_identif.value.parameters.parameters_arr
                [parser_opt->variables.new_identif.value.parameters.size] =
                parser_opt->variables.new_param;

            parser_opt->variables.new_identif.value.parameters.size++;

            return true;
        }

        // if grammar check fails
        return false;
    }
    parser_opt->return_code = STX_ERR;
    return false;
}

bool _scope_body(ParserOptions *parser_opt) {
    if (parser_opt->token.type != TOKEN_L_CRLY_BRACKET) {
        parser_opt->return_code = STX_ERR;
        return false;
    }
    _next_token(parser_opt);

    if (!_command_sequence(parser_opt)) {
        return false;
    }

    if (parser_opt->token.type != TOKEN_R_CRLY_BRACKET) {
        parser_opt->return_code = STX_ERR;
        return false;
    }
    _next_token(parser_opt);

    return true;
}

bool _command_sequence(ParserOptions *parser_opt) {
    if (parser_opt->token.type == TOKEN_R_CRLY_BRACKET) {
        return true;
    } else if (parser_opt->token.type == TOKEN_IDENTIF ||
               parser_opt->token.type == TOKEN_KEYWORD_RETURN ||
               parser_opt->token.type == TOKEN_KEYWORD_VAR ||
               parser_opt->token.type == TOKEN_KEYWORD_LET ||
               parser_opt->token.type == TOKEN_KEYWORD_IF ||
               parser_opt->token.type == TOKEN_KEYWORD_WHILE) {
        return _command(parser_opt) && _command_sequence(parser_opt);
    }
    parser_opt->return_code = STX_ERR;
    return false;
}

bool _command(ParserOptions *parser_opt) {
    if (parser_opt->token.type == TOKEN_IDENTIF) {
        _next_token(parser_opt);
        return __identif(parser_opt);
    } else if (parser_opt->token.type == TOKEN_KEYWORD_RETURN) {
        return _return_command(parser_opt);
    } else if (parser_opt->token.type == TOKEN_KEYWORD_VAR ||
               parser_opt->token.type == TOKEN_KEYWORD_LET) {
        return _variable_def(parser_opt);
    } else if (parser_opt->token.type == TOKEN_KEYWORD_IF) {
        return _conditional_command(parser_opt);
    } else if (parser_opt->token.type == TOKEN_KEYWORD_WHILE) {
        return _while_command(parser_opt);
    }
    parser_opt->return_code = STX_ERR;
    return false;
}

bool __identif(ParserOptions *parser_opt) {
    if (parser_opt->token.type == TOKEN_L_BRACKET) {
        _next_token(parser_opt);

        if (!_arg_list(parser_opt)) {
            return false;
        }

        if (parser_opt->token.type != TOKEN_R_BRACKET) {
            parser_opt->return_code = STX_ERR;
            return false;
        }
        _next_token(parser_opt);

        return true;
    } else if (parser_opt->token.type == TOKEN_ASSIGN) {
        _next_token(parser_opt);

        bool is_function;
        if (!_look_ahead_for_fn(parser_opt, &is_function)) return false;

        if (is_function) {
            return _function_call(parser_opt);
        } else if (parser_opt->token.type == TOKEN_KEYWORD_NIL) {
            _next_token(parser_opt);
            return true;
        } else {
            return parse_check_optimize_generate_expression(parser_opt);
        }
    }
    parser_opt->return_code = STX_ERR;
    return false;
}

bool _data_type(ParserOptions *parser_opt) {
    switch (parser_opt->token.type) {
        case TOKEN_KEYWORD_INT:
            parser_opt->variables.new_type = T_INT;
            _next_token(parser_opt);
            return true;
        case TOKEN_KEYWORD_INT_NIL:
            parser_opt->variables.new_type = T_INT_NIL;
            _next_token(parser_opt);
            return true;
        case TOKEN_KEYWORD_DOUBLE:
            parser_opt->variables.new_type = T_FLOAT;
            _next_token(parser_opt);
            return true;
        case TOKEN_KEYWORD_DOUBLE_NIL:
            parser_opt->variables.new_type = T_FLOAT_NIL;
            _next_token(parser_opt);
            return true;
        case TOKEN_KEYWORD_STRING:
            parser_opt->variables.new_type = T_STRING;
            _next_token(parser_opt);
            return true;
        case TOKEN_KEYWORD_STRING_NIL:
            parser_opt->variables.new_type = T_STRING_NIL;
            _next_token(parser_opt);
            return true;
        case TOKEN_KEYWORD_BOOL:
            parser_opt->variables.new_type = T_BOOL;
            _next_token(parser_opt);
            return true;
        case TOKEN_KEYWORD_BOOL_NIL:
            parser_opt->variables.new_type = T_BOOL_NIL;
            _next_token(parser_opt);
            return true;

        default:
            break;
    }

    parser_opt->return_code = STX_ERR;
    return false;
}

bool _return_command(ParserOptions *parser_opt) {
    if (parser_opt->token.type == TOKEN_KEYWORD_RETURN) {
        _next_token(parser_opt);
        return __return(parser_opt);
    }
    parser_opt->return_code = STX_ERR;
    return false;
}

bool __return(ParserOptions *parser_opt) {
    if (parser_opt->token.type == TOKEN_END_OF_FILE ||
        parser_opt->token.type == TOKEN_KEYWORD_FUNC ||
        parser_opt->token.type == TOKEN_R_CRLY_BRACKET ||
        parser_opt->token.type == TOKEN_KEYWORD_RETURN ||
        parser_opt->token.type == TOKEN_KEYWORD_VAR ||
        parser_opt->token.type == TOKEN_KEYWORD_LET ||
        parser_opt->token.type == TOKEN_KEYWORD_IF ||
        parser_opt->token.type == TOKEN_KEYWORD_WHILE) {
        return true;
    }

    if (parser_opt->token.type == TOKEN_KEYWORD_NIL) {
        _next_token(parser_opt);
        return true;
    }

    return parse_check_optimize_generate_expression(parser_opt);
}

bool _variable_def(ParserOptions *parser_opt) {
    if (parser_opt->token.type == TOKEN_KEYWORD_VAR ||
        parser_opt->token.type == TOKEN_KEYWORD_LET) {
        _next_token(parser_opt);

        if (parser_opt->token.type != TOKEN_IDENTIF) {
            parser_opt->return_code = STX_ERR;
            return false;
        }
        _next_token(parser_opt);

        return __varlet_identif(parser_opt);
    }
    parser_opt->return_code = STX_ERR;
    return false;
}

bool __varlet_identif(ParserOptions *parser_opt) {
    if (parser_opt->token.type == TOKEN_COLON) {
        _next_token(parser_opt);

        if (!_data_type(parser_opt)) {
            parser_opt->return_code = STX_ERR;
            return false;
        }

        return __varlet_identif_colon_type(parser_opt);
    } else if (parser_opt->token.type == TOKEN_ASSIGN) {
        _next_token(parser_opt);

        bool is_function;
        if (!_look_ahead_for_fn(parser_opt, &is_function)) return false;

        if (is_function) {
            return _function_call(parser_opt);
        } else if (parser_opt->token.type == TOKEN_KEYWORD_NIL) {
            _next_token(parser_opt);
            return true;
        } else {
            return parse_check_optimize_generate_expression(parser_opt);
        }
    }
    parser_opt->return_code = STX_ERR;
    return false;
}

bool __varlet_identif_colon_type(ParserOptions *parser_opt) {
    if (parser_opt->token.type == TOKEN_END_OF_FILE ||
        parser_opt->token.type == TOKEN_KEYWORD_FUNC ||
        parser_opt->token.type == TOKEN_IDENTIF ||
        parser_opt->token.type == TOKEN_R_CRLY_BRACKET ||
        parser_opt->token.type == TOKEN_KEYWORD_RETURN ||
        parser_opt->token.type == TOKEN_KEYWORD_VAR ||
        parser_opt->token.type == TOKEN_KEYWORD_LET ||
        parser_opt->token.type == TOKEN_KEYWORD_IF ||
        parser_opt->token.type == TOKEN_KEYWORD_WHILE) {
        return true;
    } else if (parser_opt->token.type == TOKEN_ASSIGN) {
        _next_token(parser_opt);

        bool is_function;
        if (!_look_ahead_for_fn(parser_opt, &is_function)) return false;

        if (is_function) {
            return _function_call(parser_opt);
        } else {
            return parse_check_optimize_generate_expression(parser_opt);
        }
    }
    parser_opt->return_code = STX_ERR;
    return false;
}

bool _conditional_command(ParserOptions *parser_opt) {
    if (parser_opt->token.type == TOKEN_KEYWORD_IF) {
        _next_token(parser_opt);
        return __if(parser_opt);
    }
    parser_opt->return_code = STX_ERR;
    return false;
}

bool __if(ParserOptions *parser_opt) {
    if (parser_opt->token.type == TOKEN_KEYWORD_LET) {
        _next_token(parser_opt);

        if (parser_opt->token.type != TOKEN_IDENTIF) {
            parser_opt->return_code = STX_ERR;
            return false;
        }
        _next_token(parser_opt);

        return _scope_body(parser_opt) && __if_let_identif_body(parser_opt);
    }

    return parse_check_optimize_generate_expression(parser_opt) &&
           _scope_body(parser_opt) && __if_let_identif_body(parser_opt);
}

bool __if_let_identif_body(ParserOptions *parser_opt) {
    if (parser_opt->token.type == TOKEN_END_OF_FILE ||
        parser_opt->token.type == TOKEN_KEYWORD_FUNC ||
        parser_opt->token.type == TOKEN_IDENTIF ||
        parser_opt->token.type == TOKEN_R_CRLY_BRACKET ||
        parser_opt->token.type == TOKEN_KEYWORD_RETURN ||
        parser_opt->token.type == TOKEN_KEYWORD_VAR ||
        parser_opt->token.type == TOKEN_KEYWORD_LET ||
        parser_opt->token.type == TOKEN_KEYWORD_IF ||
        parser_opt->token.type == TOKEN_KEYWORD_WHILE) {
        return true;
    } else if (parser_opt->token.type == TOKEN_KEYWORD_ELSE) {
        _next_token(parser_opt);
        return __if_let_identif_body_else(parser_opt);
    }
    parser_opt->return_code = STX_ERR;
    return false;
}

bool __if_let_identif_body_else(ParserOptions *parser_opt) {
    if (parser_opt->token.type == TOKEN_L_CRLY_BRACKET) {
        return _scope_body(parser_opt);
    } else if (parser_opt->token.type == TOKEN_KEYWORD_IF) {
        return _conditional_command(parser_opt);
    }
    parser_opt->return_code = STX_ERR;
    return false;
}

bool _while_command(ParserOptions *parser_opt) {
    if (parser_opt->token.type == TOKEN_KEYWORD_WHILE) {
        _next_token(parser_opt);
        return parse_check_optimize_generate_expression(parser_opt) &&
               _scope_body(parser_opt);
    }
    parser_opt->return_code = STX_ERR;
    return false;
}

bool _function_call(ParserOptions *parser_opt) {
    if (parser_opt->token.type != TOKEN_IDENTIF) {
        parser_opt->return_code = STX_ERR;
        return false;
    }
    _next_token(parser_opt);

    if (parser_opt->token.type != TOKEN_L_BRACKET) {
        parser_opt->return_code = STX_ERR;
        return false;
    }
    _next_token(parser_opt);

    if (!_arg_list(parser_opt)) {
        return false;
    }
    _next_token(parser_opt);

    if (parser_opt->token.type != TOKEN_R_BRACKET) {
        parser_opt->return_code = STX_ERR;
        return false;
    }
    _next_token(parser_opt);

    return true;
}

bool _arg_list(ParserOptions *parser_opt) {
    if (parser_opt->token.type == TOKEN_R_BRACKET) {
        return true;
    } else if (parser_opt->token.type == TOKEN_IDENTIF ||
               parser_opt->token.type == TOKEN_NUMBER ||
               parser_opt->token.type == TOKEN_STRING ||
               parser_opt->token.type == TOKEN_KEYWORD_NIL) {
        return _arg(parser_opt) && _comma_arg(parser_opt);
    }
    parser_opt->return_code = STX_ERR;
    return false;
}

bool _comma_arg(ParserOptions *parser_opt) {
    if (parser_opt->token.type == TOKEN_R_BRACKET) {
        return true;
    } else if (parser_opt->token.type == TOKEN_COMA) {
        _next_token(parser_opt);
        return _arg(parser_opt) && _comma_arg(parser_opt);
    }
    parser_opt->return_code = STX_ERR;
    return false;
}

bool _arg(ParserOptions *parser_opt) {
    if (parser_opt->token.type == TOKEN_NUMBER ||
        parser_opt->token.type == TOKEN_STRING ||
        parser_opt->token.type == TOKEN_KEYWORD_NIL) {
        return true;
    } else if (parser_opt->token.type == TOKEN_IDENTIF) {
        _next_token(parser_opt);
        return __arg_name(parser_opt);
    }
    parser_opt->return_code = STX_ERR;
    return false;
}

bool __arg_name(ParserOptions *parser_opt) {
    if (parser_opt->token.type == TOKEN_R_BRACKET ||
        parser_opt->token.type == TOKEN_COMA) {
        return true;
    } else if (parser_opt->token.type == TOKEN_COLON) {
        _next_token(parser_opt);
        return __arg_name_colon(parser_opt);
    }
    parser_opt->return_code = STX_ERR;
    return false;
}

bool __arg_name_colon(ParserOptions *parser_opt) {
    if (parser_opt->token.type == TOKEN_IDENTIF) {
        _next_token(parser_opt);
        return true;
    } else if (parser_opt->token.type == TOKEN_NUMBER ||
               parser_opt->token.type == TOKEN_STRING ||
               parser_opt->token.type == TOKEN_KEYWORD_NIL) {
        return _arg_val(parser_opt);
    }
    parser_opt->return_code = STX_ERR;
    return false;
}

bool _arg_val(ParserOptions *parser_opt) {
    if (parser_opt->token.type == TOKEN_NUMBER ||
        parser_opt->token.type == TOKEN_STRING ||
        parser_opt->token.type == TOKEN_KEYWORD_NIL) {
        _next_token(parser_opt);
        return true;
    }
    parser_opt->return_code = STX_ERR;
    return false;
}

void parse_check_optimize_generate(ParserOptions *parser_opt) {
    // get first token
    _next_token(parser_opt);

    // operate upon code
    _program(parser_opt);

    return;
}
