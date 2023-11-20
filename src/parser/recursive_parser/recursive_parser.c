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
            &parser_opt->variables.identif.value.parameters);

        // define function if identif is not in symtable and on first run, only
        // check function head
        if (parser_opt->is_first_run && _function_head(parser_opt)) {
            parser_opt->variables.identif.variant = FUNCTION;

            // add function to symtable
            STError res = st_add_element(
                parser_opt->symtable, parser_opt->variables.identif.identifier,
                parser_opt->variables.identif.return_type,
                parser_opt->variables.identif.variant,
                &(parser_opt->variables.identif.value));

            if (res != E_OK) {
                parser_opt->return_code = INTER_ERR;
                return false;
            }

            return true;
        } else if (!parser_opt->is_first_run) {
            // check function head
            if (!_function_head(parser_opt)) return false;

            // semantically check function head
            if (!analyze_function_dec(parser_opt)) return false;

            // push function scope
            // TODO think about scope identifier
            st_push_func_scope(parser_opt->symtable,
                               &parser_opt->variables.identif,
                               parser_opt->variables.identif.identifier);

            // check function body
            if (!_scope_body(parser_opt)) return false;

            // pop function scope
            st_pop_scope(parser_opt->symtable);

            // TODO generate function head
            // TODO generate function body
            //! generating will probably have to be done in recursive descent
            //! functions

            return true;
        }

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

    // check symtable for identif
    LSTElement *el =
        st_search_func(parser_opt->symtable, parser_opt->token.value.string);

    // if identif already defined on first run
    if (el != NULL && parser_opt->is_first_run) {
        parser_opt->return_code = DEF_ERR;
        return false;
    }
    // if on second run
    else if (!parser_opt->is_first_run) {
        // if function wasn't already defined on the first run
        // TODO can be removed if everything is working as it should
        if (el == NULL) {
            parser_opt->return_code = INTER_ERR;
            return false;
        }
        // if function is found, save the data for semantic analysis
        else {
            parser_opt->variables.identif = *el;
            // we have all the data and the checks were done in the first run,
            // we can stop recursive descent
            return true;
        }
    }

    // save new function name
    parser_opt->variables.identif.identifier = parser_opt->token.value.string;

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
        parser_opt->variables.identif.return_type = T_VOID;
        return true;
    } else if (parser_opt->token.type == TOKEN_ARROW) {
        if (!_next_token(parser_opt)) return false;

        // if data type grammar check passes
        if (_data_type(parser_opt)) {
            // save function type
            parser_opt->variables.identif.return_type =
                parser_opt->variables.type;
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
                parser_opt->variables.param.name = "_";
                break;
            case TOKEN_UNDERSCORE:
                parser_opt->variables.param.name =
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
                parser_opt->variables.param.identifier = "_";
                break;
            case TOKEN_UNDERSCORE:
                parser_opt->variables.param.identifier =
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
            parser_opt->variables.param.par_type = parser_opt->variables.type;

            // add param to array
            add_to_parameter_array(
                &parser_opt->variables.identif.value.parameters,
                parser_opt->variables.param);

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
        char *identif = parser_opt->token.value.string;

        if (!_next_token(parser_opt)) return false;

        if (__identif(parser_opt, identif)) {
            return true;
        }

        return false;
    } else if (parser_opt->token.type == TOKEN_KEYWORD_RETURN) {
        if (_return_command(parser_opt)) {
            return true;
        }

        return false;
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

bool __identif(ParserOptions *parser_opt, char *identif) {
    // function call
    if (parser_opt->token.type == TOKEN_L_BRACKET) {
        if (!_next_token(parser_opt)) return false;

        // init arg array
        Parameters args;
        init_parameter_array(&args);

        // save arguments
        if (!_arg_list(parser_opt, &args)) {
            destroy_parameter_array(&args);
            return false;
        }

        // semantically check function call
        if (!analyze_function_call(parser_opt, identif, &args)) {
            destroy_parameter_array(&args);
            return false;
        }

        // unallocate argument array
        destroy_parameter_array(&args);

        if (parser_opt->token.type != TOKEN_R_BRACKET) {
            parser_opt->return_code = STX_ERR;
            return false;
        }
        if (!_next_token(parser_opt)) return false;

        return true;
    }
    // assignment
    else if (parser_opt->token.type == TOKEN_ASSIGN) {
        if (!_next_token(parser_opt)) return false;

        // TODO semantically check assignment, refactor semantics into seperate
        // function

        // get called variable
        LSTElement *el = st_search_var(
            parser_opt->symtable, parser_opt->variables.identif.identifier);

        // if variable doesn't exist
        if (el == NULL) {
            parser_opt->return_code = UNDEFVAR_ERR;
            return false;
        }

        // if trying to assign to constant
        if (el->variant == CONSTANT) {
            parser_opt->return_code = OTHER_ERR;
            return false;
        }

        bool is_function;
        if (!_look_ahead_for_fn(parser_opt, &is_function)) return false;

        if (is_function) {
            return _function_call(parser_opt);
        } else {
            // TODO check expression type with variable type after precedence
            // analysis
            return parse_check_optimize_generate_expression(parser_opt);
        }
    }
    parser_opt->return_code = STX_ERR;
    return false;
}

bool _data_type(ParserOptions *parser_opt) {
    switch (parser_opt->token.type) {
        case TOKEN_KEYWORD_INT:
            parser_opt->variables.type = T_INT;
            if (!_next_token(parser_opt)) return false;
            return true;
        case TOKEN_KEYWORD_INT_NIL:
            parser_opt->variables.type = T_INT_NIL;
            if (!_next_token(parser_opt)) return false;
            return true;
        case TOKEN_KEYWORD_DOUBLE:
            parser_opt->variables.type = T_FLOAT;
            if (!_next_token(parser_opt)) return false;
            return true;
        case TOKEN_KEYWORD_DOUBLE_NIL:
            parser_opt->variables.type = T_FLOAT_NIL;
            if (!_next_token(parser_opt)) return false;
            return true;
        case TOKEN_KEYWORD_STRING:
            parser_opt->variables.type = T_STRING;
            if (!_next_token(parser_opt)) return false;
            return true;
        case TOKEN_KEYWORD_STRING_NIL:
            parser_opt->variables.type = T_STRING_NIL;
            if (!_next_token(parser_opt)) return false;
            return true;
        case TOKEN_KEYWORD_BOOL:
            parser_opt->variables.type = T_BOOL;
            if (!_next_token(parser_opt)) return false;
            return true;
        case TOKEN_KEYWORD_BOOL_NIL:
            parser_opt->variables.type = T_BOOL_NIL;
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

    // if in global scope, check fails
    if (st_is_global_active(parser_opt->symtable)) {
        //! semantic check returning syntax error
        parser_opt->return_code = STX_ERR;
        return false;
    }

    // TODO check expression type with function type after precedence analysis
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

    // save function identifier
    char *func_identif = parser_opt->token.value.string;

    if (!_next_token(parser_opt)) return false;

    if (parser_opt->token.type != TOKEN_L_BRACKET) {
        parser_opt->return_code = STX_ERR;
        return false;
    }
    if (!_next_token(parser_opt)) return false;

    // init arg array
    Parameters args;
    init_parameter_array(&args);

    // save arguments
    if (!_arg_list(parser_opt, &args)) {
        destroy_parameter_array(&args);
        return false;
    }

    // semantically check function call
    if (!analyze_function_call(parser_opt, func_identif, &args)) {
        destroy_parameter_array(&args);
        return false;
    }

    // unallocate argument array
    destroy_parameter_array(&args);

    if (!_next_token(parser_opt)) return false;

    if (parser_opt->token.type != TOKEN_R_BRACKET) {
        parser_opt->return_code = STX_ERR;
        return false;
    }
    if (!_next_token(parser_opt)) return false;

    return true;
}

bool _arg_list(ParserOptions *parser_opt, Parameters *args) {
    if (parser_opt->token.type == TOKEN_R_BRACKET) {
        return true;
    } else if (parser_opt->token.type == TOKEN_IDENTIF ||
               parser_opt->token.type == TOKEN_FLOAT ||
               parser_opt->token.type == TOKEN_INT ||
               parser_opt->token.type == TOKEN_STRING ||
               parser_opt->token.type == TOKEN_KEYWORD_NIL) {
        return _arg(parser_opt, args) && _comma_arg(parser_opt, args);
    }
    parser_opt->return_code = STX_ERR;
    return false;
}

bool _comma_arg(ParserOptions *parser_opt, Parameters *args) {
    if (parser_opt->token.type == TOKEN_R_BRACKET) {
        return true;
    } else if (parser_opt->token.type == TOKEN_COMA) {
        if (!_next_token(parser_opt)) return false;
        return _arg(parser_opt, args) && _comma_arg(parser_opt, args);
    }
    parser_opt->return_code = STX_ERR;
    return false;
}

bool _arg(ParserOptions *parser_opt, Parameters *args) {
    // init argument
    Parameter arg = {.identifier = "", .name = "", .par_type = T_VOID};

    if (parser_opt->token.type == TOKEN_FLOAT ||
        parser_opt->token.type == TOKEN_INT ||
        parser_opt->token.type == TOKEN_STRING ||
        parser_opt->token.type == TOKEN_KEYWORD_NIL) {
        switch (parser_opt->token.type) {
            // save argument type if argument is a literal
            case TOKEN_FLOAT:
                arg.par_type = T_FLOAT;
                break;

            case TOKEN_INT:
                arg.par_type = T_INT;
                break;

            case TOKEN_STRING:
                arg.par_type = T_STRING;
                break;

            case TOKEN_KEYWORD_NIL:
                arg.par_type = T_NIL;
                break;

            // never happens, here to make the compiler happy
            default:
                break;
        }

        // add argument to argument array
        add_to_parameter_array(args, arg);

        return true;
    } else if (parser_opt->token.type == TOKEN_IDENTIF) {
        // save argument identifier
        arg.identifier = parser_opt->token.value.string;

        if (!_next_token(parser_opt)) return false;
        if (!__arg_name(parser_opt, &arg)) return false;

        // add argument to argument array
        add_to_parameter_array(args, arg);
        return true;
    }
    parser_opt->return_code = STX_ERR;
    return false;
}

bool __arg_name(ParserOptions *parser_opt, Parameter *arg) {
    if (parser_opt->token.type == TOKEN_R_BRACKET ||
        parser_opt->token.type == TOKEN_COMA) {
        return true;
    } else if (parser_opt->token.type == TOKEN_COLON) {
        if (!_next_token(parser_opt)) return false;
        return __arg_name_colon(parser_opt, arg);
    }
    parser_opt->return_code = STX_ERR;
    return false;
}

bool __arg_name_colon(ParserOptions *parser_opt, Parameter *arg) {
    if (parser_opt->token.type == TOKEN_IDENTIF) {
        // save actual argument identifier, last identifier was it's name
        arg->name = arg->identifier;
        arg->identifier = parser_opt->token.value.string;
        if (!_next_token(parser_opt)) return false;
        return true;
    } else if (parser_opt->token.type == TOKEN_FLOAT ||
               parser_opt->token.type == TOKEN_INT ||
               parser_opt->token.type == TOKEN_STRING ||
               parser_opt->token.type == TOKEN_KEYWORD_NIL) {
        return _arg_val(parser_opt, arg);
    }
    parser_opt->return_code = STX_ERR;
    return false;
}

bool _arg_val(ParserOptions *parser_opt, Parameter *arg) {
    if (parser_opt->token.type == TOKEN_INT ||
        parser_opt->token.type == TOKEN_FLOAT ||
        parser_opt->token.type == TOKEN_STRING ||
        parser_opt->token.type == TOKEN_KEYWORD_NIL) {
        switch (parser_opt->token.type) {
            // save argument type if argument is a literal
            case TOKEN_FLOAT:
                arg->par_type = T_FLOAT;
                break;

            case TOKEN_INT:
                arg->par_type = T_INT;
                break;

            case TOKEN_STRING:
                arg->par_type = T_STRING;
                break;

            case TOKEN_KEYWORD_NIL:
                arg->par_type = T_NIL;
                break;

            // never happens, here to make the compiler happy
            default:
                break;
        }
        if (!_next_token(parser_opt)) return false;
        return true;
    }
    parser_opt->return_code = STX_ERR;
    return false;
}

void add_inbuilt_functions_to_symtable(ListST *symtable) {
    // TODO:
    symtable = symtable;
}

void parse_function_definition(ParserOptions *parser_opt) {
    // get first token
    if (!_first_token(parser_opt)) return;

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
