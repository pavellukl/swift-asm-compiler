#include "recursive_parser.h"

bool _look_ahead_for_fn(ParserOptions *parser_opt, bool *is_function) {
    if (parser_opt->token.type != TOKEN_IDENTIF) {
        *is_function = false;
        return true;
    }

    LSTElement *el = st_search_element(parser_opt->symtable,
                                       parser_opt->token.value.string, NULL);

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
               parser_opt->token.type == TOKEN_KEYWORD_WHILE ||
               parser_opt->token.type == TOKEN_IDENTIF) {
        return _command(parser_opt) && _program(parser_opt);
    }
    parser_opt->return_code = STX_ERR;
    return false;
}

bool _function_definition(ParserOptions *parser_opt) {
    //! in first run add function to symtable
    //! in second run generate and run semantic check
    if (parser_opt->token.type == TOKEN_KEYWORD_FUNC) {
        // define function if identif is not in symtable and on first run, only
        // check function head
        if (parser_opt->is_first_run) {
            LSTElement func = {
                .variant = FUNCTION, .return_type = T_VOID, .identifier = NULL};

            // initialize parameter array for function
            init_parameter_array(&func.value.parameters);

            if (!_function_head(parser_opt, &func, NULL)) {
                destroy_parameter_array(&func.value.parameters);
                free(func.identifier);
                return false;
            }

            // add function to symtable
            STError err = st_add_element(parser_opt->symtable, func.identifier,
                                         func.return_type, func.variant,
                                         func.value, true);

            if (err != E_OK) {
                parser_opt->return_code = INTER_ERR;
                return false;
            }

            return true;
        } else if (!parser_opt->is_first_run) {
            LSTElement *func;

            // check function head
            if (!_function_head(parser_opt, NULL, &func)) return false;

            // semantically check function head
            if (!analyze_function_dec(parser_opt, &func->value.parameters)) {
                return false;
            }

            // save current scope counter
            int tmp_scope_n = parser_opt->gen_var.scope_n;
            // push function scope
            STError err = st_push_func_scope(parser_opt->symtable, func,
                                             ++parser_opt->gen_var.scope_n);
            if (err != E_OK) {
                parser_opt->return_code = INTER_ERR;
                return false;
            }

            // set current function of semantic context
            parser_opt->sem_ctx.current_fnc = func;
            parser_opt->sem_ctx.has_function_all_returns = false;

            // check function body
            bool scope_body_res = _scope_body(parser_opt);

            // if a non void function is missing return
            if (scope_body_res && func->return_type != T_VOID &&
                !parser_opt->sem_ctx.has_function_all_returns) {
                parser_opt->return_code = FNCALL_ERR;
                return false;
            }

            // reset current function of semantic context
            parser_opt->sem_ctx.current_fnc = NULL;
            parser_opt->sem_ctx.has_function_all_returns = false;

            // pop function scope
            st_pop_scope(parser_opt->symtable);
            // restore saved state of scope count
            parser_opt->gen_var.scope_n = tmp_scope_n;

            // TODO generate function head
            // TODO generate function body
            //! generating will probably have to be done in recursive descent
            //! functions

            return scope_body_res;
        }

        return false;
    }

    parser_opt->return_code = STX_ERR;
    return false;
}

bool _function_head(ParserOptions *parser_opt, LSTElement *func,
                    LSTElement **func_ptr) {
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
    LSTElement *el = st_search_element(parser_opt->symtable,
                                       parser_opt->token.value.string, NULL);

    // if identif already defined on first run
    if (el != NULL && parser_opt->is_first_run) {
        parser_opt->return_code = DEF_ERR;
        return false;
    }
    // if on second run
    else if (!parser_opt->is_first_run) {
        // if function wasn't already defined on the first run
        // TODO can be removed if everything is working as it should
        if (el == NULL || el->variant != FUNCTION) {
            parser_opt->return_code = INTER_ERR;
            return false;
        }
        // if function is found, save the data for semantic analysis
        else {
            *func_ptr = el;
            // we have all the data and the checks were done in the first run,
            // we can stop recursive descent once we consume all tokens until
            // left curly bracket
            while (parser_opt->token.type != TOKEN_L_CRLY_BRACKET) {
                if (!_next_token(parser_opt)) {
                    return false;
                };
            }

            return true;
        }
    }

    // save new function name
    if (!clone_string(&func->identifier, parser_opt->token.value.string)) {
        parser_opt->return_code = INTER_ERR;
        return false;
    };

    if (!_next_token(parser_opt)) return false;
    if (parser_opt->token.type != TOKEN_L_BRACKET) {
        parser_opt->return_code = STX_ERR;
        return false;
    }
    if (!_next_token(parser_opt)) return false;
    if (!_param_list(parser_opt, func)) return false;

    if (parser_opt->token.type != TOKEN_R_BRACKET) {
        parser_opt->return_code = STX_ERR;
        return false;
    }
    if (!_next_token(parser_opt)) return false;

    return __func_identif_lbracket_arglist_rbracket(parser_opt, func);
}

bool __func_identif_lbracket_arglist_rbracket(ParserOptions *parser_opt,
                                              LSTElement *func) {
    if (parser_opt->token.type == TOKEN_L_CRLY_BRACKET) {
        // save function type as void when return type omitted
        func->return_type = T_VOID;
        return true;
    } else if (parser_opt->token.type == TOKEN_ARROW) {
        if (!_next_token(parser_opt)) return false;

        // save function type
        if (!_data_type(parser_opt, &func->return_type)) return false;

        return true;
    }
    parser_opt->return_code = STX_ERR;
    return false;
}

bool _param_list(ParserOptions *parser_opt, LSTElement *func) {
    if (parser_opt->token.type == TOKEN_R_BRACKET) {
        return true;
    } else if (parser_opt->token.type == TOKEN_IDENTIF ||
               parser_opt->token.type == TOKEN_UNDERSCORE) {
        return _param(parser_opt, func) && _comma_param(parser_opt, func);
    }
    parser_opt->return_code = STX_ERR;
    return false;
}

bool _comma_param(ParserOptions *parser_opt, LSTElement *func) {
    if (parser_opt->token.type == TOKEN_R_BRACKET) {
        return true;
    } else if (parser_opt->token.type == TOKEN_COMA) {
        if (!_next_token(parser_opt)) return false;
        return _param(parser_opt, func) && _comma_param(parser_opt, func);
    }
    parser_opt->return_code = STX_ERR;
    return false;
}

bool _param(ParserOptions *parser_opt, LSTElement *func) {
    if (parser_opt->token.type == TOKEN_IDENTIF ||
        parser_opt->token.type == TOKEN_UNDERSCORE) {
        Parameter param = {.identifier = NULL, .name = NULL};

        // save parameter name to new parameter
        switch (parser_opt->token.type) {
            case TOKEN_IDENTIF:
                // save name identifier
                if (!clone_string(&param.name,
                                  parser_opt->token.value.string)) {
                    parser_opt->return_code = INTER_ERR;
                    return false;
                };

                break;
            case TOKEN_UNDERSCORE:
                param.name = NULL;
                break;
            default:
                break;
        }

        if (!_next_token(parser_opt)) {
            free(param.name);
            return false;
        }

        if (!__param_name(parser_opt, &param)) {
            free(param.name);
            free(param.identifier);
            return false;
        };

        // add param to array
        if (!add_to_parameter_array(&func->value.parameters, param)) {
            free(param.name);
            free(param.identifier);
            parser_opt->return_code = INTER_ERR;
            return false;
        }

        return true;
    }
    parser_opt->return_code = STX_ERR;
    return false;
}

bool __param_name(ParserOptions *parser_opt, Parameter *param) {
    if (parser_opt->token.type == TOKEN_IDENTIF ||
        parser_opt->token.type == TOKEN_UNDERSCORE) {
        // save parameter identifier to new parameter
        switch (parser_opt->token.type) {
            case TOKEN_IDENTIF:
                // save identifier
                if (!clone_string(&param->identifier,
                                  parser_opt->token.value.string)) {
                    parser_opt->return_code = INTER_ERR;
                    return false;
                };
                break;
            case TOKEN_UNDERSCORE:
                param->identifier = NULL;
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

        // save param type
        if (!_data_type(parser_opt, &param->par_type)) return false;

        return true;
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
        char *identif;
        // save identifier
        if (!clone_string(&identif, parser_opt->token.value.string)) {
            parser_opt->return_code = INTER_ERR;
            return false;
        };

        if (!_next_token(parser_opt)) {
            free(identif);
            return false;
        }

        if (!__identif(parser_opt, identif)) {
            free(identif);
            return false;
        }

        // free token string value
        free(identif);
        return true;
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

bool __identif(ParserOptions *parser_opt, char *identif) {
    // function call
    if (parser_opt->token.type == TOKEN_L_BRACKET) {
        if (!_next_token(parser_opt)) return false;

        // init arg array
        Arguments args;
        init_argument_array(&args);

        // save arguments
        if (!_arg_list(parser_opt, &args)) {
            destroy_argument_array(&args);
            return false;
        }

        // generation is done here
        // semantically check function call
        if (!analyze_generate_function_call(parser_opt, identif, &args, NULL)) {
            destroy_argument_array(&args);
            return false;
        }

        // unallocate argument array
        destroy_argument_array(&args);

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

        bool is_function;
        if (!_look_ahead_for_fn(parser_opt, &is_function)) return false;

        if (is_function) {
            // fake experession node
            ASTNode expression_node = {.data_type = T_VOID};
            ASTNode *expression_node_ptr = &expression_node;

            if (!_function_call(parser_opt, &expression_node_ptr->data_type))
                return false;

            // semantically check assignment
            if (!analyze_assignment(parser_opt, identif, expression_node_ptr,
                                    true)) {
                return false;
            }

            return true;

        } else {
            ASTNode *expression_node;
            if (!parse_check_optimize_expression(parser_opt,
                                                 &expression_node)) {
                return false;
            }

            // semantically check assignment
            if (!analyze_assignment(parser_opt, identif, expression_node,
                                    false)) {
                // TODO: fix double free
                // _free_AST(expression_node);
                return false;
            }

            // TODO: fix double free
            // _free_AST(expression_node);

            return true;
        }
    }
    parser_opt->return_code = STX_ERR;
    return false;
}

bool _data_type(ParserOptions *parser_opt, Type *type) {
    switch (parser_opt->token.type) {
        case TOKEN_KEYWORD_INT:
            *type = T_INT;
            if (!_next_token(parser_opt)) return false;
            return true;
        case TOKEN_KEYWORD_INT_NIL:
            *type = T_INT_NIL;
            if (!_next_token(parser_opt)) return false;
            return true;
        case TOKEN_KEYWORD_DOUBLE:
            *type = T_FLOAT;
            if (!_next_token(parser_opt)) return false;
            return true;
        case TOKEN_KEYWORD_DOUBLE_NIL:
            *type = T_FLOAT_NIL;
            if (!_next_token(parser_opt)) return false;
            return true;
        case TOKEN_KEYWORD_STRING:
            *type = T_STRING;
            if (!_next_token(parser_opt)) return false;
            return true;
        case TOKEN_KEYWORD_STRING_NIL:
            *type = T_STRING_NIL;
            if (!_next_token(parser_opt)) return false;
            return true;
        case TOKEN_KEYWORD_BOOL:
            *type = T_BOOL;
            if (!_next_token(parser_opt)) return false;
            return true;
        case TOKEN_KEYWORD_BOOL_NIL:
            *type = T_BOOL_NIL;
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

        ASTNode expression_node = {.data_type = T_VOID};
        ASTNode *expression_node_ptr = &expression_node;
        if (!__return(parser_opt, &expression_node_ptr)) return false;

        // semantically check return statement
        if (!analyze_return(parser_opt, parser_opt->sem_ctx.current_fnc,
                            expression_node_ptr)) {
            // TODO why double free???
            // _free_AST(expression_node_ptr);
            return false;
        }
        // TODO: fix double free
        // _free_AST(expression_node_ptr);

        parser_opt->sem_ctx.has_function_all_returns = true;

        return true;
    }
    parser_opt->return_code = STX_ERR;
    return false;
}

bool __return(ParserOptions *parser_opt, ASTNode **expression_node) {
    if (parser_opt->token.type == TOKEN_END_OF_FILE ||
        parser_opt->token.type == TOKEN_KEYWORD_FUNC ||
        parser_opt->token.type == TOKEN_R_CRLY_BRACKET ||
        parser_opt->token.type == TOKEN_KEYWORD_RETURN ||
        parser_opt->token.type == TOKEN_KEYWORD_VAR ||
        parser_opt->token.type == TOKEN_KEYWORD_LET ||
        parser_opt->token.type == TOKEN_KEYWORD_IF ||
        parser_opt->token.type == TOKEN_KEYWORD_WHILE) {
        // since no expression was provided after return, type stays void
        return true;
    }

    // save expression type
    if (!parse_check_optimize_expression(parser_opt, expression_node)) {
        return false;
    }

    return true;
}

bool _variable_def(ParserOptions *parser_opt) {
    if (parser_opt->token.type == TOKEN_KEYWORD_VAR ||
        parser_opt->token.type == TOKEN_KEYWORD_LET) {
        // save if defined variable is a variable or constant
        bool is_constant = true;
        if (parser_opt->token.type == TOKEN_KEYWORD_VAR) is_constant = false;

        if (!_next_token(parser_opt)) return false;

        if (parser_opt->token.type != TOKEN_IDENTIF) {
            parser_opt->return_code = STX_ERR;
            return false;
        }

        // save defined varible identifier
        // it gets added to symtable, no freeing required
        char *identif;
        if (!clone_string(&identif, parser_opt->token.value.string)) {
            parser_opt->return_code = INTER_ERR;
            return false;
        };

        if (!_next_token(parser_opt)) {
            free(identif);
            return false;
        }

        Type expected_var_type = T_VOID;
        ASTNode provided_expression_node = {.data_type = T_VOID};
        ASTNode *provided_expression_node_ptr = &provided_expression_node;
        // TODO handle generation differently according to this bool
        bool is_function = false;

        if (!__varlet_identif(parser_opt, &expected_var_type,
                              &provided_expression_node_ptr, &is_function)) {
            free(identif);
            // TODO double free fix
            // _free_AST(provided_expression_node_ptr);
            return false;
        }

        // do semantic actions on variable definition
        if (!analyze_var_def(parser_opt, is_constant, identif,
                             expected_var_type, provided_expression_node_ptr,
                             is_function)) {
            free(identif);
            // TODO double free fix
            // _free_AST(provided_expression_node_ptr);
            return false;
        }

        // TODO double free fix
        // _free_AST(provided_expression_node_ptr);

        return true;
    }
    parser_opt->return_code = STX_ERR;
    return false;
}

bool __varlet_identif(ParserOptions *parser_opt, Type *expected_var_type,
                      ASTNode **provided_expression_node, bool *is_function) {
    if (parser_opt->token.type == TOKEN_COLON) {
        if (!_next_token(parser_opt)) return false;

        // save expected variable type
        if (!_data_type(parser_opt, expected_var_type)) {
            parser_opt->return_code = STX_ERR;
            return false;
        }

        return __varlet_identif_colon_type(parser_opt, provided_expression_node,
                                           is_function);
    } else if (parser_opt->token.type == TOKEN_ASSIGN) {
        if (!_next_token(parser_opt)) return false;

        if (!_look_ahead_for_fn(parser_opt, is_function)) return false;

        if (*is_function) {
            // save provided value type, in this case it's the return type of
            // the function
            if (!_function_call(parser_opt,
                                &(*provided_expression_node)->data_type))
                return false;

            return true;
        } else {
            // save expression type as provided value type
            if (!parse_check_optimize_expression(parser_opt,
                                                 provided_expression_node)) {
                return false;
            }

            return true;
        }
    }
    parser_opt->return_code = STX_ERR;
    return false;
}

bool __varlet_identif_colon_type(ParserOptions *parser_opt,
                                 ASTNode **provided_expression_node,
                                 bool *is_function) {
    if (parser_opt->token.type == TOKEN_END_OF_FILE ||
        parser_opt->token.type == TOKEN_KEYWORD_FUNC ||
        parser_opt->token.type == TOKEN_IDENTIF ||
        parser_opt->token.type == TOKEN_R_CRLY_BRACKET ||
        parser_opt->token.type == TOKEN_KEYWORD_RETURN ||
        parser_opt->token.type == TOKEN_KEYWORD_VAR ||
        parser_opt->token.type == TOKEN_KEYWORD_LET ||
        parser_opt->token.type == TOKEN_KEYWORD_IF ||
        parser_opt->token.type == TOKEN_KEYWORD_WHILE) {
        // provided value type is void, which it's already initialized to, no
        // changes needed
        return true;
    } else if (parser_opt->token.type == TOKEN_ASSIGN) {
        if (!_next_token(parser_opt)) return false;

        if (!_look_ahead_for_fn(parser_opt, is_function)) return false;

        if (*is_function) {
            // save provided value type, in this case it's the return type of
            // the function
            if (!_function_call(parser_opt,
                                &(*provided_expression_node)->data_type))
                return false;

            return true;
        } else {
            // save expression type as provided value type
            if (!parse_check_optimize_expression(parser_opt,
                                                 provided_expression_node)) {
                return false;
            }

            return true;
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
    // TODO handle defined values of variables in branches
    if (parser_opt->token.type == TOKEN_KEYWORD_LET) {
        if (!_next_token(parser_opt)) return false;

        if (parser_opt->token.type != TOKEN_IDENTIF) {
            parser_opt->return_code = STX_ERR;
            return false;
        }

        char *identifier;
        // save identifier
        if (!clone_string(&identifier, parser_opt->token.value.string)) {
            parser_opt->return_code = INTER_ERR;
            return false;
        };

        if (!_next_token(parser_opt)) {
            free(identifier);
            return false;
        }

        // save current scope counter
        int tmp_scope_n = parser_opt->gen_var.scope_n;
        // push if scope
        STError err =
            st_push_scope(parser_opt->symtable, ++parser_opt->gen_var.scope_n);
        if (err != E_OK) {
            free(identifier);
            parser_opt->return_code = INTER_ERR;
            return false;
        }

        Type initial_var_type = T_VOID;
        LSTElement *el;
        if (!analyze_if_let(parser_opt, identifier, &initial_var_type, &el)) {
            free(identifier);
            return false;
        }

        // free token string value
        free(identifier);

        // TODO
        bool has_returns_tmp = parser_opt->sem_ctx.has_function_all_returns;
        parser_opt->sem_ctx.has_function_all_returns = false;

        if (!_scope_body(parser_opt)) return false;

        bool has_if_return = parser_opt->sem_ctx.has_function_all_returns;

        // pop if scope
        st_pop_scope(parser_opt->symtable);

        // restore initial type of var
        el->return_type = initial_var_type;
        // restore saved state of scope count
        parser_opt->gen_var.scope_n = tmp_scope_n;

        parser_opt->sem_ctx.has_function_all_returns = false;
        bool has_else_branch = false;
        if (!__if_let_identif_body(parser_opt, &has_else_branch)) return false;

        if (has_else_branch) {
            parser_opt->sem_ctx.has_function_all_returns =
                (has_if_return &&
                 parser_opt->sem_ctx.has_function_all_returns) ||
                has_returns_tmp;
        } else {
            parser_opt->sem_ctx.has_function_all_returns = has_returns_tmp;
        }

        // TODO collect data for checking if all branches have return and check
        // here

        return true;
    }

    ASTNode *expression_node;
    if (!parse_check_optimize_expression(parser_opt, &expression_node)) {
        return false;
    }

    // semantically analyze if (check if expression is of type bool)
    if (expression_node->data_type != T_BOOL) {
        // TODO: fix double free
        // _free_AST(expression_node);
        parser_opt->return_code = EXPRTYPE_ERR;
        return false;
    }
    // TODO: fix double free
    // _free_AST(expression_node);

    // save current scope counter
    int tmp_scope_n = parser_opt->gen_var.scope_n;
    // push if scope
    STError err =
        st_push_scope(parser_opt->symtable, ++parser_opt->gen_var.scope_n);

    if (err != E_OK) {
        parser_opt->return_code = INTER_ERR;
        return false;
    }

    // TODO

    bool has_returns_tmp = parser_opt->sem_ctx.has_function_all_returns;
    parser_opt->sem_ctx.has_function_all_returns = false;

    if (!_scope_body(parser_opt)) return false;

    bool has_if_return = parser_opt->sem_ctx.has_function_all_returns;

    // pop if scope
    st_pop_scope(parser_opt->symtable);
    // restore saved state of scope count
    parser_opt->gen_var.scope_n = tmp_scope_n;

    // TODO
    parser_opt->sem_ctx.has_function_all_returns = false;
    bool has_else_branch = false;
    if (!__if_let_identif_body(parser_opt, &has_else_branch)) return false;

    if (has_else_branch) {
        parser_opt->sem_ctx.has_function_all_returns =
            (has_if_return && parser_opt->sem_ctx.has_function_all_returns) ||
            has_returns_tmp;
    } else {
        parser_opt->sem_ctx.has_function_all_returns = has_returns_tmp;
    }

    return true;
}

bool __if_let_identif_body(ParserOptions *parser_opt, bool *has_else_branch) {
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
        return __if_let_identif_body_else(parser_opt, has_else_branch);
    }
    parser_opt->return_code = STX_ERR;
    return false;
}

bool __if_let_identif_body_else(ParserOptions *parser_opt,
                                bool *has_else_branch) {
    if (parser_opt->token.type == TOKEN_L_CRLY_BRACKET) {
        // save current scope counter
        int tmp_scope_n = parser_opt->gen_var.scope_n;
        // push if scope
        STError err =
            st_push_scope(parser_opt->symtable, ++parser_opt->gen_var.scope_n);
        if (err != E_OK) {
            parser_opt->return_code = INTER_ERR;
            return false;
        }

        *has_else_branch = true;

        bool has_returns_tmp = parser_opt->sem_ctx.has_function_all_returns;
        parser_opt->sem_ctx.has_function_all_returns = false;

        if (!_scope_body(parser_opt)) return false;

        parser_opt->sem_ctx.has_function_all_returns =
            has_returns_tmp || parser_opt->sem_ctx.has_function_all_returns;

        // pop if scope
        st_pop_scope(parser_opt->symtable);
        // restore saved state of scope count
        parser_opt->gen_var.scope_n = tmp_scope_n;

        return true;
    } else if (parser_opt->token.type == TOKEN_KEYWORD_IF) {
        return _conditional_command(parser_opt);
    }
    parser_opt->return_code = STX_ERR;
    return false;
}

bool _while_command(ParserOptions *parser_opt) {
    if (parser_opt->token.type == TOKEN_KEYWORD_WHILE) {
        if (!_next_token(parser_opt)) return false;

        ASTNode *expression_node;
        if (!parse_check_optimize_expression(parser_opt, &expression_node)) {
            return false;
        }

        // semantically analyze while (check if expression is of type bool)
        if (expression_node->data_type != T_BOOL) {
            // TODO: fix double free
            //_free_AST(expression_node);
            parser_opt->return_code = EXPRTYPE_ERR;
            return false;
        }

        // TODO: fix double free
        // _free_AST(expression_node);

        // save current scope counter
        int tmp_scope_n = parser_opt->gen_var.scope_n;
        // push while scope
        STError err =
            st_push_scope(parser_opt->symtable, ++parser_opt->gen_var.scope_n);
        if (err != E_OK) {
            parser_opt->return_code = INTER_ERR;
            return false;
        }

        if (!_scope_body(parser_opt)) return false;

        // pop while scope
        st_pop_scope(parser_opt->symtable);
        // restore saved state of scope count
        parser_opt->gen_var.scope_n = tmp_scope_n;

        return true;
    }
    parser_opt->return_code = STX_ERR;
    return false;
}

bool _function_call(ParserOptions *parser_opt, Type *return_type) {
    if (parser_opt->token.type != TOKEN_IDENTIF) {
        parser_opt->return_code = STX_ERR;
        return false;
    }

    // save function identifier
    char *func_identif;
    if (!clone_string(&func_identif, parser_opt->token.value.string)) {
        parser_opt->return_code = INTER_ERR;
        return false;
    };

    if (!_next_token(parser_opt)) {
        free(func_identif);
        return false;
    }

    if (parser_opt->token.type != TOKEN_L_BRACKET) {
        free(func_identif);
        parser_opt->return_code = STX_ERR;
        return false;
    }

    if (!_next_token(parser_opt)) {
        free(func_identif);
        return false;
    }

    // init arg array
    Arguments args;
    init_argument_array(&args);

    // save arguments
    if (!_arg_list(parser_opt, &args)) {
        free(func_identif);
        destroy_argument_array(&args);
        return false;
    }

    // semantically check function call
    if (!analyze_generate_function_call(parser_opt, func_identif, &args,
                                        return_type)) {
        free(func_identif);
        destroy_argument_array(&args);
        return false;
    }

    // free token string value
    free(func_identif);
    // unallocate argument array
    destroy_argument_array(&args);

    if (parser_opt->token.type != TOKEN_R_BRACKET) {
        parser_opt->return_code = STX_ERR;
        return false;
    }
    if (!_next_token(parser_opt)) return false;

    return true;
}

bool _arg_list(ParserOptions *parser_opt, Arguments *args) {
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

bool _comma_arg(ParserOptions *parser_opt, Arguments *args) {
    if (parser_opt->token.type == TOKEN_R_BRACKET) {
        return true;
    } else if (parser_opt->token.type == TOKEN_COMA) {
        if (!_next_token(parser_opt)) return false;
        return _arg(parser_opt, args) && _comma_arg(parser_opt, args);
    }
    parser_opt->return_code = STX_ERR;
    return false;
}

bool _arg(ParserOptions *parser_opt, Arguments *args) {
    // init argument
    Argument arg = {
        .name = NULL, .identifier = NULL, .par_type = T_VOID, .value = {0}};

    if (parser_opt->token.type == TOKEN_FLOAT ||
        parser_opt->token.type == TOKEN_INT ||
        parser_opt->token.type == TOKEN_STRING ||
        parser_opt->token.type == TOKEN_BOOL ||
        parser_opt->token.type == TOKEN_KEYWORD_NIL) {
        switch (parser_opt->token.type) {
                // save argument type if argument is a literal and value
            case TOKEN_FLOAT:
                arg.par_type = T_FLOAT;
                arg.token_type = TOKEN_FLOAT;
                arg.value = parser_opt->token.value;
                break;

            case TOKEN_INT:
                arg.par_type = T_INT;
                arg.token_type = TOKEN_INT;
                arg.value = parser_opt->token.value;
                break;

            case TOKEN_STRING:
                arg.par_type = T_STRING;
                arg.token_type = TOKEN_STRING;
                arg.value = parser_opt->token.value;
                break;

            case TOKEN_BOOL:
                arg.par_type = T_BOOL;
                arg.token_type = TOKEN_BOOL;
                arg.value = parser_opt->token.value;
                break;

            case TOKEN_KEYWORD_NIL:
                arg.par_type = T_NIL;
                arg.token_type = TOKEN_KEYWORD_NIL;
                break;

            // never happens, here to make the compiler happy
            default:
                break;
        }

        if (!_next_token(parser_opt)) return false;

        // add argument to argument array
        if (!add_to_argument_array(args, arg)) {
            parser_opt->return_code = INTER_ERR;
            return false;
        };

        return true;
    } else if (parser_opt->token.type == TOKEN_IDENTIF) {
        // save argument name
        if (!clone_string(&arg.name, parser_opt->token.value.string)) {
            parser_opt->return_code = INTER_ERR;
            return false;
        };

        if (!_next_token(parser_opt)) {
            free(arg.name);
            return false;
        }

        bool is_name_identifier = false;
        if (!__arg_name(parser_opt, &arg, &is_name_identifier)) {
            free(arg.name);
            return false;
        }

        if (is_name_identifier) {
            arg.identifier = arg.name;
            arg.name = NULL;
        }

        // add argument to argument array
        if (!add_to_argument_array(args, arg)) {
            free(arg.name);
            free(arg.identifier);
            parser_opt->return_code = INTER_ERR;
            return false;
        };

        return true;
    }
    parser_opt->return_code = STX_ERR;
    return false;
}

bool __arg_name(ParserOptions *parser_opt, Argument *arg,
                bool *is_name_identifier) {
    if (parser_opt->token.type == TOKEN_R_BRACKET ||
        parser_opt->token.type == TOKEN_COMA) {
        *is_name_identifier = true;
        return true;
    } else if (parser_opt->token.type == TOKEN_COLON) {
        if (!_next_token(parser_opt)) return false;
        return __arg_name_colon(parser_opt, arg);
    }
    parser_opt->return_code = STX_ERR;
    return false;
}

bool __arg_name_colon(ParserOptions *parser_opt, Argument *arg) {
    if (parser_opt->token.type == TOKEN_IDENTIF) {
        // save argument identifier
        if (!clone_string(&arg->identifier, parser_opt->token.value.string)) {
            parser_opt->return_code = INTER_ERR;
            return false;
        };

        if (!_next_token(parser_opt)) {
            return false;
        }

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

bool _arg_val(ParserOptions *parser_opt, Argument *arg) {
    if (parser_opt->token.type == TOKEN_INT ||
        parser_opt->token.type == TOKEN_FLOAT ||
        parser_opt->token.type == TOKEN_STRING ||
        parser_opt->token.type == TOKEN_BOOL ||
        parser_opt->token.type == TOKEN_KEYWORD_NIL) {
        switch (parser_opt->token.type) {
            // save argument type if argument is a literal and value
            case TOKEN_FLOAT:
                arg->par_type = T_FLOAT;
                arg->token_type = TOKEN_FLOAT;
                arg->value = parser_opt->token.value;
                break;

            case TOKEN_INT:
                arg->par_type = T_INT;
                arg->token_type = TOKEN_INT;
                arg->value = parser_opt->token.value;
                break;

            case TOKEN_STRING:
                arg->par_type = T_STRING;
                arg->token_type = TOKEN_STRING;
                arg->value = parser_opt->token.value;
                break;

            case TOKEN_BOOL:
                arg->par_type = T_BOOL;
                arg->token_type = TOKEN_BOOL;
                arg->value = parser_opt->token.value;
                break;

            case TOKEN_KEYWORD_NIL:
                arg->par_type = T_NIL;
                arg->token_type = TOKEN_KEYWORD_NIL;
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

bool add_inbuilt_functions_to_symtable(ListST *symtable) {
    LSTElementValue value;

    init_parameter_array(&value.parameters);
    char *func_identifier = NULL;
    Parameter param = {.name = NULL, .identifier = NULL, .par_type = T_VOID};

    // readString()
    if (!clone_string(&func_identifier, "readString")) return false;
    if (st_add_element(symtable, func_identifier, T_STRING_NIL, FUNCTION, value,
                       true) != E_OK) {
        free(func_identifier);
        return false;
    }

    // readInt()
    if (!clone_string(&func_identifier, "readInt")) return false;
    if (st_add_element(symtable, func_identifier, T_INT_NIL, FUNCTION, value,
                       true) != E_OK) {
        free(func_identifier);
        return false;
    }

    // readDouble()
    if (!clone_string(&func_identifier, "readDouble")) return false;
    if (st_add_element(symtable, func_identifier, T_FLOAT_NIL, FUNCTION, value,
                       true) != E_OK) {
        free(func_identifier);
        return false;
    }

    // write(term1, term2, ..., termN)
    if (!clone_string(&func_identifier, "write")) return false;
    value.parameters.infinite = true;

    if (st_add_element(symtable, func_identifier, T_VOID, FUNCTION, value,
                       true) != E_OK) {
        free(func_identifier);
        return false;
    }

    // Int2Double(_term: Int)
    if (!clone_string(&func_identifier, "Int2Double")) return false;
    init_parameter_array(&value.parameters);

    param.name = NULL;
    if (!clone_string(&param.identifier, "term")) {
        free(func_identifier);
        return false;
    }
    param.par_type = T_INT;

    if (!add_to_parameter_array(&value.parameters, param)) {
        free(func_identifier);
        free(param.identifier);
        free(param.name);
        return false;
    }

    if (st_add_element(symtable, func_identifier, T_FLOAT, FUNCTION, value,
                       true) != E_OK) {
        free(func_identifier);
        destroy_parameter_array(&value.parameters);
        return false;
    }

    // Double2Int(_term: Double)
    if (!clone_string(&func_identifier, "Double2Int")) return false;
    init_parameter_array(&value.parameters);

    param.name = NULL;
    if (!clone_string(&param.identifier, "term")) {
        free(func_identifier);
        return false;
    }
    param.par_type = T_FLOAT;

    if (!add_to_parameter_array(&value.parameters, param)) {
        free(func_identifier);
        free(param.identifier);
        free(param.name);
        return false;
    }

    if (st_add_element(symtable, func_identifier, T_INT, FUNCTION, value,
                       true) != E_OK) {
        free(func_identifier);
        destroy_parameter_array(&value.parameters);
        return false;
    }

    // length(_s: String)
    if (!clone_string(&func_identifier, "length")) return false;
    init_parameter_array(&value.parameters);

    param.name = NULL;
    if (!clone_string(&param.identifier, "s")) {
        free(func_identifier);
        return false;
    }
    param.par_type = T_STRING;

    if (!add_to_parameter_array(&value.parameters, param)) {
        free(func_identifier);
        free(param.identifier);
        free(param.name);
        return false;
    }

    if (st_add_element(symtable, func_identifier, T_INT, FUNCTION, value,
                       true) != E_OK) {
        free(func_identifier);
        destroy_parameter_array(&value.parameters);
        return false;
    }

    // substring(of s: String, startingAt i: Int, endingBefore j: Int)
    if (!clone_string(&func_identifier, "substring")) return false;
    init_parameter_array(&value.parameters);

    if (!clone_string(&param.name, "of")) {
        free(func_identifier);
        return false;
    }
    if (!clone_string(&param.identifier, "s")) {
        free(func_identifier);
        free(param.name);
        return false;
    }
    param.par_type = T_STRING;

    if (!add_to_parameter_array(&value.parameters, param)) {
        free(func_identifier);
        free(param.identifier);
        free(param.name);
        return false;
    }

    if (!clone_string(&param.name, "startingAt")) {
        free(func_identifier);
        return false;
    }
    if (!clone_string(&param.identifier, "i")) {
        free(func_identifier);
        free(param.name);
        return false;
    }
    param.par_type = T_INT;

    if (!add_to_parameter_array(&value.parameters, param)) {
        free(func_identifier);
        free(param.identifier);
        free(param.name);
        return false;
    }

    if (!clone_string(&param.name, "endingBefore")) {
        free(func_identifier);
        return false;
    }
    if (!clone_string(&param.identifier, "j")) {
        free(func_identifier);
        free(param.name);
        return false;
    }
    param.par_type = T_INT;

    if (!add_to_parameter_array(&value.parameters, param)) {
        free(func_identifier);
        free(param.identifier);
        free(param.name);
        return false;
    }

    if (st_add_element(symtable, func_identifier, T_STRING_NIL, FUNCTION, value,
                       true) != E_OK) {
        free(func_identifier);
        destroy_parameter_array(&value.parameters);
        return false;
    }

    // ord(_c: String)
    if (!clone_string(&func_identifier, "ord")) return false;
    init_parameter_array(&value.parameters);

    param.name = NULL;
    if (!clone_string(&param.identifier, "c")) {
        free(func_identifier);
        free(param.name);
        return false;
    }
    param.par_type = T_STRING;

    if (!add_to_parameter_array(&value.parameters, param)) {
        free(func_identifier);
        free(param.identifier);
        free(param.name);
        return false;
    }

    if (st_add_element(symtable, func_identifier, T_INT, FUNCTION, value,
                       true) != E_OK) {
        destroy_parameter_array(&value.parameters);
        return false;
    }

    // chr(_i: Int)
    if (!clone_string(&func_identifier, "chr")) return false;
    init_parameter_array(&value.parameters);

    param.name = NULL;
    if (!clone_string(&param.identifier, "i")) {
        free(func_identifier);
        free(param.name);
        return false;
    }
    param.par_type = T_INT;

    if (!add_to_parameter_array(&value.parameters, param)) {
        free(func_identifier);
        free(param.identifier);
        free(param.name);
        return false;
    }

    if (st_add_element(symtable, func_identifier, T_STRING, FUNCTION, value,
                       true) != E_OK) {
        destroy_parameter_array(&value.parameters);
        return false;
    }

    return true;
}

void parse_function_definition(ParserOptions *parser_opt) {
    // get first token
    if (!_first_token(parser_opt)) return;

    while (true) {
        // search functions and EOF
        while (parser_opt->token.type != TOKEN_KEYWORD_FUNC &&
               parser_opt->token.type != TOKEN_END_OF_FILE) {
            if(!_next_token(parser_opt)) return;
        }
        // if EOF -> end with success
        if (parser_opt->token.type == TOKEN_END_OF_FILE) {
            parser_opt->return_code = OK;
            return;
        };

        // initialize return code to OK
        parser_opt->return_code = OK;
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
