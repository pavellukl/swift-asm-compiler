#include "recursive_parser.h"

bool _program(ParserOptions *parser_opt) {
    if (parser_opt->token.type == TOKEN_END_OF_FILE) {
        _next_token(parser_opt);
        return true;
    } else if (parser_opt->token.type == TOKEN_KEYWORD_FUNC) {
        return _function_definition(parser_opt) && _program(parser_opt);
    } else if (parser_opt->token.type == TOKEN_KEYWORD_FUNC
            || parser_opt->token.type == TOKEN_KEYWORD_VAR
            || parser_opt->token.type == TOKEN_KEYWORD_LET
            || parser_opt->token.type == TOKEN_KEYWORD_IF
            || parser_opt->token.type == TOKEN_KEYWORD_WHILE
    ) {
        return _command(parser_opt) && _program(parser_opt);
    }
    parser_opt->return_code = STX_ERR;
    return false;
}

bool _function_definition(ParserOptions *parser_opt) {
    //! in first run only generate function (do not check)
    //! in second run only semantically check function (do not generate)
    //! view 'parser_opt->is_first_run' for info about run
    if (parser_opt->token.type == TOKEN_KEYWORD_FUNC) {
        return _function_head(parser_opt) && _scope_body(parser_opt);
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
        return true;
    } else if (parser_opt->token.type == TOKEN_ARROW) {
        _next_token(parser_opt);
        return _data_type(parser_opt);
    }
    parser_opt->return_code = STX_ERR;
    return false;
}

bool _param_list(ParserOptions *parser_opt) {
    if (parser_opt->token.type == TOKEN_R_BRACKET) {
        return true;
    } else if (parser_opt->token.type == TOKEN_IDENTIF
        || parser_opt->token.type == TOKEN_UNDERSCORE) {
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
    if (parser_opt->token.type == TOKEN_IDENTIF
        || parser_opt->token.type == TOKEN_UNDERSCORE) {
        _next_token(parser_opt);
        return __param_name(parser_opt);
    }
    parser_opt->return_code = STX_ERR;
    return false;
}

bool __param_name(ParserOptions *parser_opt) {
    if (parser_opt->token.type == TOKEN_IDENTIF
        || parser_opt->token.type == TOKEN_UNDERSCORE) {
        _next_token(parser_opt);
        if (parser_opt->token.type != TOKEN_COLON) {
            parser_opt->return_code = STX_ERR;
            return false;
        }
        _next_token(parser_opt);
        return _data_type(parser_opt);
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
    } else if (parser_opt->token.type == TOKEN_IDENTIF
            || parser_opt->token.type == TOKEN_KEYWORD_RETURN
            || parser_opt->token.type == TOKEN_KEYWORD_VAR
            || parser_opt->token.type == TOKEN_KEYWORD_LET
            || parser_opt->token.type == TOKEN_KEYWORD_IF
            || parser_opt->token.type == TOKEN_KEYWORD_WHILE ) {
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
    } else if (parser_opt->token.type == TOKEN_KEYWORD_VAR 
            || parser_opt->token.type == TOKEN_KEYWORD_LET) {
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
        // TODO: expression || function call
        // _IDENTIF → TOKEN_ASSIGN EXPRESSION
        // _IDENTIF → TOKEN_ASSIGN FUNCTION_CALL
        return true;
    }
    parser_opt->return_code = STX_ERR;
    return false;
}

bool _data_type(ParserOptions *parser_opt) {
    if (parser_opt->token.type == TOKEN_KEYWORD_INT
        || parser_opt->token.type == TOKEN_KEYWORD_INT_NIL
        || parser_opt->token.type == TOKEN_KEYWORD_DOUBLE
        || parser_opt->token.type == TOKEN_KEYWORD_DOUBLE_NIL
        || parser_opt->token.type == TOKEN_KEYWORD_STRING
        || parser_opt->token.type == TOKEN_KEYWORD_STRING_NIL
        || parser_opt->token.type == TOKEN_KEYWORD_BOOL
        || parser_opt->token.type == TOKEN_KEYWORD_BOOL_NIL ) {
        // *data_type = parser_opt->token.type;
        _next_token(parser_opt);
        return true;
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
    if (parser_opt->token.type == TOKEN_END_OF_FILE
        || parser_opt->token.type == TOKEN_KEYWORD_FUNC
        || parser_opt->token.type == TOKEN_R_CRLY_BRACKET
        || parser_opt->token.type == TOKEN_KEYWORD_RETURN
        || parser_opt->token.type == TOKEN_KEYWORD_VAR
        || parser_opt->token.type == TOKEN_KEYWORD_LET
        || parser_opt->token.type == TOKEN_KEYWORD_IF
        || parser_opt->token.type == TOKEN_KEYWORD_WHILE) {
        return true;
    }
    // TODO: expression || sth else (not function call)
    // else if
    //     _RETURN → EXPRESSION

    parser_opt->return_code = STX_ERR;
    return false;
}

bool _variable_def(ParserOptions *parser_opt) {
    if (parser_opt->token.type == TOKEN_KEYWORD_VAR
        || parser_opt->token.type == TOKEN_KEYWORD_LET) {
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
        // TODO: expression || function call
        return true;
    }
    parser_opt->return_code = STX_ERR;
    return false;
}

bool __varlet_identif_colon_type(ParserOptions *parser_opt) {
    if (parser_opt->token.type == TOKEN_END_OF_FILE
        || parser_opt->token.type == TOKEN_KEYWORD_FUNC
        || parser_opt->token.type == TOKEN_IDENTIF
        || parser_opt->token.type == TOKEN_R_CRLY_BRACKET
        || parser_opt->token.type == TOKEN_KEYWORD_RETURN
        || parser_opt->token.type == TOKEN_KEYWORD_VAR
        || parser_opt->token.type == TOKEN_KEYWORD_LET
        || parser_opt->token.type == TOKEN_KEYWORD_IF
        || parser_opt->token.type == TOKEN_KEYWORD_WHILE) {
        return true;
    } else if (parser_opt->token.type == TOKEN_ASSIGN) {
        _next_token(parser_opt);
        // TODO: expression || function call
        return true;
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
    
    return parse_check_optimize_generate_expression(parser_opt)
            && _scope_body(parser_opt)
            && __if_let_identif_body(parser_opt);
}

bool __if_let_identif_body(ParserOptions *parser_opt) {
    if (parser_opt->token.type == TOKEN_END_OF_FILE
        || parser_opt->token.type == TOKEN_KEYWORD_FUNC
        || parser_opt->token.type == TOKEN_IDENTIF
        || parser_opt->token.type == TOKEN_R_CRLY_BRACKET
        || parser_opt->token.type == TOKEN_KEYWORD_RETURN
        || parser_opt->token.type == TOKEN_KEYWORD_VAR
        || parser_opt->token.type == TOKEN_KEYWORD_LET
        || parser_opt->token.type == TOKEN_KEYWORD_IF
        || parser_opt->token.type == TOKEN_KEYWORD_WHILE) {
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
        return parse_check_optimize_generate_expression(parser_opt)
                && _scope_body(parser_opt);
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
    } else if (parser_opt->token.type == TOKEN_IDENTIF
            || parser_opt->token.type == TOKEN_NUMBER
            || parser_opt->token.type == TOKEN_STRING
            || parser_opt->token.type == TOKEN_KEYWORD_NIL) {
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
    if (parser_opt->token.type == TOKEN_NUMBER
        || parser_opt->token.type == TOKEN_STRING
        || parser_opt->token.type == TOKEN_KEYWORD_NIL) {
        return true;
    } else if (parser_opt->token.type == TOKEN_IDENTIF) {
        _next_token(parser_opt);
        return __arg_name(parser_opt);
    }
    parser_opt->return_code = STX_ERR;
    return false;
}

bool __arg_name(ParserOptions *parser_opt) {
    if (parser_opt->token.type == TOKEN_R_BRACKET
        || parser_opt->token.type == TOKEN_COMA) {
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
    } else if (parser_opt->token.type == TOKEN_NUMBER
            || parser_opt->token.type == TOKEN_STRING
            || parser_opt->token.type == TOKEN_KEYWORD_NIL) {
        return _arg_val(parser_opt);
    }
    parser_opt->return_code = STX_ERR;
    return false;
}

bool _arg_val(ParserOptions *parser_opt) {
    if (parser_opt->token.type == TOKEN_NUMBER
        || parser_opt->token.type == TOKEN_STRING
        || parser_opt->token.type == TOKEN_KEYWORD_NIL) {
        _next_token(parser_opt);
        return true;
    }
    parser_opt->return_code = STX_ERR;
    return false;
}

void parse_function_definition(ParserOptions *parser_opt) {
    // get first token
    _next_token(parser_opt);

    while (true) {
        // search functions and EOF
        while (parser_opt->token.type != TOKEN_KEYWORD_FUNC && parser_opt->token.type != TOKEN_END_OF_FILE) {
            _next_token(parser_opt);
        }
        // if EOF -> end with success
        if (parser_opt->token.type == TOKEN_END_OF_FILE) {
            parser_opt->return_code = OK;
            return;
        };

        // operate upon function definition
        _function_definition(parser_opt);
        switch (parser_opt->return_code)
        {
            case OK: break;
            default: return;
        } 
    }
}

void parse_check_optimize_generate(ParserOptions *parser_opt) {
    // get first token
    _next_token(parser_opt);

    // operate upon code (skip function definitions)
    _program(parser_opt);

    return;
}
