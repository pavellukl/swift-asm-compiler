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
        if (!_next_token(parser_opt)) return false;
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
    //! in first run add function to symtable
    //! in second run generate and run semantic check
    if (parser_opt->token.type == TOKEN_KEYWORD_FUNC) {
        // invalidate old parameter array before checking function
        invalidate_parameter_array(
            &parser_opt->variables.new_identif.value.parameters);

        if (_function_head(parser_opt) && _scope_body(parser_opt)) {
            // define function if identif is not in symtable and on first run
            if (parser_opt->is_first_run) {
                parser_opt->variables.new_identif.variant = FUNCTION;

                // add function to symtable
                STError res = st_add_element(
                    parser_opt->symtable,
                    parser_opt->variables.new_identif.identifier,
                    parser_opt->variables.new_identif.return_type,
                    parser_opt->variables.new_identif.variant,
                    &(parser_opt->variables.new_identif.value));

                if (res != E_OK) {
                    parser_opt->return_code = INTER_ERR;
                    return false;
                }
            } else {
                // semantically check function if on second run
                analyze_function_dec(parser_opt);
                // TODO check semantic return code

                // TODO generate function in target code
            }

            return true;
        };
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
    if (!_next_token(parser_opt)) return false;
    if (parser_opt->token.type != TOKEN_IDENTIF) {
        parser_opt->return_code = STX_ERR;
        return false;
    }

    // TODO only get function identifier on second run and then find
    // it in symtable

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

    if (!_next_token(parser_opt)) return false;
    if (parser_opt->token.type != TOKEN_L_BRACKET) {
        parser_opt->return_code = STX_ERR;
        return false;
    }
    if (!_next_token(parser_opt)) return false;
    if (!_param_list(parser_opt)) {
        return false;
    }
    if (parser_opt->token.type != TOKEN_R_BRACKET) {
        parser_opt->return_code = STX_ERR;
        return false;
    }
    if (!_next_token(parser_opt)) return false;

    return __func_identif_lbracket_arglist_rbracket(parser_opt);
}

bool __func_identif_lbracket_arglist_rbracket(ParserOptions *parser_opt) {
    if (parser_opt->token.type == TOKEN_L_CRLY_BRACKET) {
        // save function type as void when return type omitted
        parser_opt->variables.new_identif.return_type = T_VOID;
        return true;
    } else if (parser_opt->token.type == TOKEN_ARROW) {
        if (!_next_token(parser_opt)) return false;

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
        if (!_next_token(parser_opt)) return false;
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

        if (!_next_token(parser_opt)) return false;
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

        if (!_next_token(parser_opt)) return false;
        if (parser_opt->token.type != TOKEN_COLON) {
            parser_opt->return_code = STX_ERR;
            return false;
        }
        if (!_next_token(parser_opt)) return false;

        // if data type grammar check passes
        if (_data_type(parser_opt)) {
            // save function type
            parser_opt->variables.new_param.par_type =
                parser_opt->variables.new_type;

            // add param to array
            add_to_parameter_array(
                &parser_opt->variables.new_identif.value.parameters,
                parser_opt->variables.new_param);

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
    if (!_next_token(parser_opt)) return false;

    if (!_command_sequence(parser_opt)) {
        return false;
    }

    if (parser_opt->token.type != TOKEN_R_CRLY_BRACKET) {
        parser_opt->return_code = STX_ERR;
        return false;
    }
    if (!_next_token(parser_opt)) return false;

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
        if (!_next_token(parser_opt)) return false;
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
        if (!_next_token(parser_opt)) return false;

        if (!_arg_list(parser_opt)) {
            return false;
        }

        if (parser_opt->token.type != TOKEN_R_BRACKET) {
            parser_opt->return_code = STX_ERR;
            return false;
        }
        if (!_next_token(parser_opt)) return false;

        return true;
    } else if (parser_opt->token.type == TOKEN_ASSIGN) {
        if (!_next_token(parser_opt)) return false;

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

bool _data_type(ParserOptions *parser_opt) {
    switch (parser_opt->token.type) {
        case TOKEN_KEYWORD_INT:
            parser_opt->variables.new_type = T_INT;
            if (!_next_token(parser_opt)) return false;
            return true;
        case TOKEN_KEYWORD_INT_NIL:
            parser_opt->variables.new_type = T_INT_NIL;
            if (!_next_token(parser_opt)) return false;
            return true;
        case TOKEN_KEYWORD_DOUBLE:
            parser_opt->variables.new_type = T_FLOAT;
            if (!_next_token(parser_opt)) return false;
            return true;
        case TOKEN_KEYWORD_DOUBLE_NIL:
            parser_opt->variables.new_type = T_FLOAT_NIL;
            if (!_next_token(parser_opt)) return false;
            return true;
        case TOKEN_KEYWORD_STRING:
            parser_opt->variables.new_type = T_STRING;
            if (!_next_token(parser_opt)) return false;
            return true;
        case TOKEN_KEYWORD_STRING_NIL:
            parser_opt->variables.new_type = T_STRING_NIL;
            if (!_next_token(parser_opt)) return false;
            return true;
        case TOKEN_KEYWORD_BOOL:
            parser_opt->variables.new_type = T_BOOL;
            if (!_next_token(parser_opt)) return false;
            return true;
        case TOKEN_KEYWORD_BOOL_NIL:
            parser_opt->variables.new_type = T_BOOL_NIL;
            if (!_next_token(parser_opt)) return false;
            return true;

        default:
            break;
    }

    parser_opt->return_code = STX_ERR;
    return false;
}

bool _return_command(ParserOptions *parser_opt) {
    if (parser_opt->token.type == TOKEN_KEYWORD_RETURN) {
        if (!_next_token(parser_opt)) return false;
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

    return parse_check_optimize_generate_expression(parser_opt);
}

bool _variable_def(ParserOptions *parser_opt) {
    if (parser_opt->token.type == TOKEN_KEYWORD_VAR ||
        parser_opt->token.type == TOKEN_KEYWORD_LET) {
        if (!_next_token(parser_opt)) return false;

        if (parser_opt->token.type != TOKEN_IDENTIF) {
            parser_opt->return_code = STX_ERR;
            return false;
        }
        if (!_next_token(parser_opt)) return false;

        return __varlet_identif(parser_opt);
    }
    parser_opt->return_code = STX_ERR;
    return false;
}

bool __varlet_identif(ParserOptions *parser_opt) {
    if (parser_opt->token.type == TOKEN_COLON) {
        if (!_next_token(parser_opt)) return false;

        if (!_data_type(parser_opt)) {
            parser_opt->return_code = STX_ERR;
            return false;
        }

        return __varlet_identif_colon_type(parser_opt);
    } else if (parser_opt->token.type == TOKEN_ASSIGN) {
        if (!_next_token(parser_opt)) return false;

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
        if (!_next_token(parser_opt)) return false;

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
        if (!_next_token(parser_opt)) return false;
        return __if(parser_opt);
    }
    parser_opt->return_code = STX_ERR;
    return false;
}

bool __if(ParserOptions *parser_opt) {
    if (parser_opt->token.type == TOKEN_KEYWORD_LET) {
        if (!_next_token(parser_opt)) return false;

        if (parser_opt->token.type != TOKEN_IDENTIF) {
            parser_opt->return_code = STX_ERR;
            return false;
        }
        if (!_next_token(parser_opt)) return false;

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
        if (!_next_token(parser_opt)) return false;
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
        if (!_next_token(parser_opt)) return false;
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
    if (!_next_token(parser_opt)) return false;

    if (parser_opt->token.type != TOKEN_L_BRACKET) {
        parser_opt->return_code = STX_ERR;
        return false;
    }
    if (!_next_token(parser_opt)) return false;

    if (!_arg_list(parser_opt)) {
        return false;
    }
    if (!_next_token(parser_opt)) return false;

    if (parser_opt->token.type != TOKEN_R_BRACKET) {
        parser_opt->return_code = STX_ERR;
        return false;
    }
    if (!_next_token(parser_opt)) return false;

    return true;
}

bool _arg_list(ParserOptions *parser_opt) {
    if (parser_opt->token.type == TOKEN_R_BRACKET) {
        return true;
    } else if (parser_opt->token.type == TOKEN_IDENTIF ||
               parser_opt->token.type == TOKEN_FLOAT ||
               parser_opt->token.type == TOKEN_INT ||
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
        if (!_next_token(parser_opt)) return false;
        return _arg(parser_opt) && _comma_arg(parser_opt);
    }
    parser_opt->return_code = STX_ERR;
    return false;
}

bool _arg(ParserOptions *parser_opt) {
    if (parser_opt->token.type == TOKEN_FLOAT ||
        parser_opt->token.type == TOKEN_INT ||
        parser_opt->token.type == TOKEN_STRING ||
        parser_opt->token.type == TOKEN_KEYWORD_NIL) {
        return true;
    } else if (parser_opt->token.type == TOKEN_IDENTIF) {
        if (!_next_token(parser_opt)) return false;
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
        if (!_next_token(parser_opt)) return false;
        return __arg_name_colon(parser_opt);
    }
    parser_opt->return_code = STX_ERR;
    return false;
}

bool __arg_name_colon(ParserOptions *parser_opt) {
    if (parser_opt->token.type == TOKEN_IDENTIF) {
        if (!_next_token(parser_opt)) return false;
        return true;
    } else if (parser_opt->token.type == TOKEN_FLOAT ||
               parser_opt->token.type == TOKEN_INT ||
               parser_opt->token.type == TOKEN_STRING ||
               parser_opt->token.type == TOKEN_KEYWORD_NIL) {
        return _arg_val(parser_opt);
    }
    parser_opt->return_code = STX_ERR;
    return false;
}

bool _arg_val(ParserOptions *parser_opt) {
    if (parser_opt->token.type == TOKEN_INT ||
        parser_opt->token.type == TOKEN_FLOAT ||
        parser_opt->token.type == TOKEN_STRING ||
        parser_opt->token.type == TOKEN_KEYWORD_NIL) {
        if (!_next_token(parser_opt)) return false;
        return true;
    }
    parser_opt->return_code = STX_ERR;
    return false;
}

void parse_function_definition(ParserOptions *parser_opt) {
    // get first token
    if (!_next_token(parser_opt)) return;

    while (true) {
        // search functions and EOF
        while (parser_opt->token.type != TOKEN_KEYWORD_FUNC &&
               parser_opt->token.type != TOKEN_END_OF_FILE) {
            _next_token(parser_opt);
        }
        // if EOF -> end with success
        if (parser_opt->token.type == TOKEN_END_OF_FILE) {
            parser_opt->return_code = OK;
            return;
        };

        // operate upon function definition
        _function_definition(parser_opt);
        switch (parser_opt->return_code) {
            case OK:
                break;
            default:
                return;
        }
    }
}

void parse_check_optimize_generate(ParserOptions *parser_opt) {
    // get first token
    if (!_next_token(parser_opt)) return;

    // operate upon code
    _program(parser_opt);

    return;
}
