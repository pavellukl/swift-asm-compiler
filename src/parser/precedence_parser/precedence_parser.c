#include "precedence_parser.h"

void _free_AST(ASTNode *node) {
    // TODO: free tokens
    if (node->left != NULL) {
        _free_AST(node->left);
    }
    if (node->right != NULL) {
        _free_AST(node->right);
    }
    free(node);
}


void _free_pp_list(ListPP *list) {
    list_pp_first(list);
    PPListItem data;
    while (list_pp_get_value(list, &data)) {
        _free_AST(data.node);
        list_pp_next(list);
    }
    list_pp_dispose(list);
}

bool _get_token_types(TokenData token, ParserOptions *parser_opt, PPListItemType *pp_type, Type *data_type) {
    switch (token.type)
    {
        case TOKEN_ADD:
            pp_type = TERMINAL_ADD;
            return true;
        case TOKEN_SUB:
            *pp_type = TERMINAL_SUB;
            return true;
        case TOKEN_MUL:
            *pp_type = TERMINAL_MUL;
            return true;
        case TOKEN_DIV:
            *pp_type = TERMINAL_DIV;
            return true;
        case TOKEN_EQUAL:
            *pp_type = TERMINAL_EQUAL;
            return true;
        case TOKEN_NOT_EQUAL:
            *pp_type = TERMINAL_NOT_EQUAL;
            return true;
        case TOKEN_LESSER:
            *pp_type = TERMINAL_LESSER;
            return true;
        case TOKEN_LESSER_EQUAL:
            *pp_type = TERMINAL_LESSER_EQUAL;
            return true;
        case TOKEN_GREATER:
            *pp_type = TERMINAL_GREATER;
            return true;
        case TOKEN_GREATER_EQUAL:
            *pp_type = TERMINAL_GREATER_EQUAL;
            return true;
        case TOKEN_AND:
            *pp_type = TERMINAL_AND;
            return true;
        case TOKEN_OR:
            *pp_type = TERMINAL_OR;
            return true;
        case TOKEN_EXCL_MARK:
            *pp_type = TERMINAL_EXCL_MARK;
            return true;
        case TOKEN_NOT:
            *pp_type = TERMINAL_NOT;
            return true;
        case TOKEN_NIL_COALESCING:
            *pp_type = TERMINAL_NIL_COALESCING;
            return true;
        case TOKEN_INT:
            *pp_type = TERMINAL_INT;
            *data_type = T_INT;
            return true;
        case TOKEN_FLOAT:
            *pp_type = TERMINAL_FLOAT;
            *data_type = T_FLOAT;
            return true;
        case TOKEN_STRING:
            *pp_type = TERMINAL_STRING;
            *data_type = T_STRING;
            return true;
        case TOKEN_BOOL:
            *pp_type = TERMINAL_BOOL;
            *data_type = T_BOOL;
            return true;
        case TOKEN_IDENTIF:
            *pp_type = TERMINAL_IDENTIF;
            LSTElement *el = st_search_element(parser_opt->symtable, token.value.string);
            if (el == NULL || (el->variant != FUNCTION && el->defined_value == false)) {
                parser_opt->return_code = UNDEFVAR_ERR;
                return false;
            } else if (el->variant == FUNCTION) {
                parser_opt->return_code = OTHER_ERR;
                return false;
            }
            *data_type = el->return_type;
            return true;
        case TOKEN_KEYWORD_NIL:
            *pp_type = TERMINAL_KEYWORD_NIL;
            *data_type = T_NIL;
            return true;
        case TOKEN_L_BRACKET:
            *pp_type = TERMINAL_L_BRACKET;
            return true;
        case TOKEN_R_BRACKET:
            *pp_type = TERMINAL_R_BRACKET;
            return true;
        default:
            *pp_type = TERMINAL_EMPTY;
            return true;
    }
}

Type _remove_nilable(Type *type) {
    switch (*type)
    {
        case T_INT_NIL:
            return T_INT;
        case T_FLOAT_NIL:
            return T_FLOAT;
        case T_STRING_NIL:
            return T_STRING;
        case T_BOOL_NIL:
            return T_BOOL;
        default:
            // do not insert anything apart types above
            return T_INT;
    }
}

bool _is_number_type(Type type) {
    return type == T_INT ||
           type == T_FLOAT ||
           type == T_INT_NIL ||
           type == T_FLOAT_NIL;
}

bool _is_nilable_type(Type type) {
    return type == T_INT_NIL ||
           type == T_FLOAT_NIL ||
           type == T_STRING_NIL ||
           type == T_BOOL_NIL;
}

bool _build_rule_result(ParserOptions *parser_opt, PPListItem *items, int rule_r_size, PPListItem *new_item) {
    // non/terminal type
    new_item->pp_type = NONTERMINAL_EXPRESSION;

    switch (rule_r_size)
    {
        case 1:
            // E -> i (items[2])
            // no data type tests (every data type is ok)
            // build new_item
            new_item->node->token = items[2].node->token;
            new_item->node->left = NULL;
            new_item->node->right = NULL;
            if (items[2].pp_type == TERMINAL_IDENTIF) {
                LSTElement *el = st_search_element(parser_opt->symtable, items[2].node->token.value.string);
                if (el == NULL) {
                    parser_opt->return_code = UNDEFVAR_ERR;
                    return false;
                }
                new_item->data_type = el->return_type;
            } else {
                new_item->data_type = items[2].data_type;
            }
            break;
        case 2:
            if (items[1].pp_type == TERMINAL_NOT) {
                // E -> TOKEN_NOT E (items[1] items[2])
                // data type tests
                if (items[2].data_type != T_BOOL) {
                    parser_opt->return_code = EXPRTYPE_ERR;
                    return false;
                }

                // build new_item
                new_item->node->token = items[1].node->token;
                new_item->node->left = items[2].node;
                new_item->node->right = NULL;
                new_item->data_type = items[2].data_type;
            } else {
                // E -> E TOKEN_EXCL_MARK (items[1] items[2])
                // no data type tests (every data type is ok)
                // build new_item
                *new_item = items[1];
                _remove_nilable(&new_item->data_type);
            }
            break;
        case 3:
            if (items[0].pp_type == TERMINAL_L_BRACKET) {
                // E -> TOKEN_L_BRACKET E TOKEN_R_BRACKET (items[0] items[1] items[2])
                // no data type tests (every data type is ok)
                // build new_item
                *new_item = items[1];
            } else {
                // E -> E OPERATOR E (items[0] items[1] items[2])
                // data type tests and new data type resolving
                switch (items[1].node->token.type)
                {
                    case TOKEN_ADD:
                        // both should be only int or float or string (not nilable)
                        if ((items[0].data_type != T_STRING || items[2].data_type != T_STRING)
                            &&
                            (!_is_number_type(items[0].data_type) ||
                            !_is_number_type(items[2].data_type ||
                            _is_nilable_type(items[0].data_type) ||
                            _is_nilable_type(items[2].data_type)))) {
                            parser_opt->return_code = EXPRTYPE_ERR;
                            return false;
                        }

                        if (items[0].data_type == T_FLOAT || items[2].data_type == T_FLOAT) {
                            new_item->data_type = T_FLOAT;
                        } else {
                            new_item->data_type = items[0].data_type;
                        }
                        break;
                    case TOKEN_SUB:
                    case TOKEN_MUL:
                    case TOKEN_DIV:
                        // both should be only int or float (not nilable)
                        if (!_is_number_type(items[0].data_type) ||
                            !_is_number_type(items[2].data_type ||
                            _is_nilable_type(items[0].data_type) ||
                            _is_nilable_type(items[2].data_type))) {
                            parser_opt->return_code = EXPRTYPE_ERR;
                            return false;
                        }

                        if (items[0].data_type == T_FLOAT || items[2].data_type == T_FLOAT) {
                            new_item->data_type = T_FLOAT;
                        } else {
                            new_item->data_type = items[0].data_type;
                        }
                        break;
                    case TOKEN_LESSER:
                    case TOKEN_LESSER_EQUAL:
                    case TOKEN_GREATER:
                    case TOKEN_GREATER_EQUAL:
                        // both should be of the same type (int and float are same)
                        // operands can't be nilable type (not nil as well)
                        // operands can't be bool
                        if ((items[0].data_type != items[2].data_type)
                            &&
                            (!_is_number_type(items[0].data_type) ||
                            !_is_number_type(items[2].data_type))) {
                            parser_opt->return_code = EXPRTYPE_ERR;
                            return false;
                        }
                        if (_is_nilable_type(items[0].data_type) ||
                            _is_nilable_type(items[2].data_type) ||
                            items[0].data_type == T_BOOL) {
                            parser_opt->return_code = EXPRTYPE_ERR;
                            return false;
                        }
                        break;
                    case TOKEN_EQUAL:
                    case TOKEN_NOT_EQUAL:
                        // both should be of the same type (int and float are same)
                        // operands can be nilable type (nil as well)
                        if ((items[0].data_type != items[2].data_type)
                            &&
                            (!_is_number_type(items[0].data_type) ||
                            !_is_number_type(items[2].data_type))) {
                            parser_opt->return_code = EXPRTYPE_ERR;
                            return false;
                        }
                        new_item->data_type = T_BOOL;
                        break;
                    case TOKEN_AND:
                    case TOKEN_OR:
                        // operands must be bool
                        // operands can't be nilable type (not nil as well)
                        if (items[0].data_type != T_BOOL ||
                            items[2].data_type != T_BOOL) {
                            parser_opt->return_code = EXPRTYPE_ERR;
                            return false;
                        }
                        new_item->data_type = T_BOOL;
                        break;
                    case TOKEN_NIL_COALESCING:
                        // right operand must be of the same type as left operand (but without nilable part)
                        // operands can't be nil
                        if (items[0].data_type == T_NIL ||
                            items[0].data_type !=
                                _remove_nilable(&items[2].data_type)) {
                            parser_opt->return_code = EXPRTYPE_ERR;
                            return false;
                        }
                        new_item->data_type = items[2].data_type;
                        break;
                    default:
                        parser_opt->return_code = STX_ERR;
                        return false;
                }

                // build new_item (data type already set)
                new_item->node->token = items[1].node->token;
                new_item->node->left = items[0].node;
                new_item->node->right = items[2].node;
            }
            break;
    }

    return true;
}

bool _is_simple_expression(PPListItem item) {
    return (item.pp_type == TERMINAL_IDENTIF
           || item.pp_type == TERMINAL_INT
           || item.pp_type == TERMINAL_FLOAT
           || item.pp_type == TERMINAL_STRING
           || item.pp_type == TERMINAL_BOOL);
}

bool _is_binary_operator(PPListItem item) {
    return (item.pp_type == TERMINAL_ADD
           || item.pp_type == TERMINAL_SUB
           || item.pp_type == TERMINAL_MUL
           || item.pp_type == TERMINAL_DIV
           || item.pp_type == TERMINAL_EQUAL
           || item.pp_type == TERMINAL_NOT_EQUAL
           || item.pp_type == TERMINAL_LESSER
           || item.pp_type == TERMINAL_LESSER_EQUAL
           || item.pp_type == TERMINAL_GREATER
           || item.pp_type == TERMINAL_GREATER_EQUAL
           || item.pp_type == TERMINAL_AND
           || item.pp_type == TERMINAL_OR
           || item.pp_type == TERMINAL_NIL_COALESCING);
}

bool _rule_exists(ParserOptions *parser_opt, PPListItem *items, int rule_r_size) {
    // if the size doesn't match with any rule -> return failure
    if (rule_r_size < 0 || rule_r_size > 3) {
        parser_opt->return_code = STX_ERR;
        return false;
    }

    // check if a rule exists
    switch (rule_r_size)
    {
        case 1:
            if (!_is_simple_expression(items[2])) {
                parser_opt->return_code = STX_ERR;
                return false;
            }
            break;
        case 2:
            if ((items[1].pp_type != NONTERMINAL_EXPRESSION
                    || items[2].pp_type != TERMINAL_EXCL_MARK)
                && (items[1].pp_type != TERMINAL_NOT
                    || items[2].pp_type != NONTERMINAL_EXPRESSION)) {
                parser_opt->return_code = STX_ERR;
                return false;
            }
            break;
        case 3:
            if ((items[0].pp_type != TERMINAL_L_BRACKET
                    || items[1].pp_type != NONTERMINAL_EXPRESSION
                    || items[2].pp_type != TERMINAL_R_BRACKET)
                && (items[0].pp_type != NONTERMINAL_EXPRESSION
                    || !_is_binary_operator(items[1])
                    || items[2].pp_type != NONTERMINAL_EXPRESSION)) {
                parser_opt->return_code = STX_ERR;
                return false;
            }
            break;
    }

    return true;
}

int _calculate_rule_r_size(ParserOptions *parser_opt, ListPP *list, int *size) {
    PPListItem item;
    *size = 0;

    list_pp_first(list);
    list_pp_get_value(list, &item);
    while (item.pp_type != FLAG_HANDLE && item.pp_type != TERMINAL_EMPTY ) {
        (*size)++;
        list_pp_next(list);
        list_pp_get_value(list, &item);
    }

    // if no handle was found -> return failure
    if (item.pp_type == TERMINAL_EMPTY) {
        parser_opt->return_code = STX_ERR;
        return false;
    }

    return true;
}

bool _reduce_list_until_handle(ParserOptions *parser_opt, ListPP *list) {
    // calculate the size of the right side of the rule
    int rule_r_size;
    if(!_calculate_rule_r_size(parser_opt, list, &rule_r_size)) return false;

    // load all possible rule parts (max 3)
    PPListItem items[3];
    list_pp_first(list);
    list_pp_get_value(list, &items[3]);
    list_pp_next(list);
    list_pp_get_value(list, &items[2]);
    list_pp_next(list);
    list_pp_get_value(list, &items[2]);

    // check whether rule exists
    if (!_rule_exists(parser_opt, items, rule_r_size)) return false;

    // get rule result
    PPListItem new_item;
    if (!_build_rule_result(parser_opt, items, rule_r_size, &new_item)) return false;

    // pop rule with handle
    PPListItem item;
    list_pp_get_first(list, &item);
    while (item.pp_type != FLAG_HANDLE) {
        list_pp_delete_first(list);
        list_pp_get_first(list, &item);
    }
    list_pp_delete_first(list);  // deletes handle

    // add result of the rule to the list
    if (list_pp_insert_first(list, new_item) == LIST_ALLOC_ERR) return false;

    return true;
}

bool _list_contains_done_sequence(ListPP *list) {
    PPListItem item;

    // first item should be EXPRESSION
    list_pp_first(list);
    list_pp_get_value(list, &item);
    if (item.pp_type != NONTERMINAL_EXPRESSION) return false;

    // second item should be bottom of the list
    list_pp_next(list);
    list_pp_get_value(list, &item);
    if (item.pp_type != TERMINAL_EMPTY) return false;

    // third item shouldn't exist
    list_pp_next(list);
    if (list_pp_is_active(list)) return false;

    return true;
}

PPListItem _get_first_terminal_item(ListPP *list) {
    PPListItem item;

    list_pp_first(list);
    list_pp_get_value(list, &item);
    while (item.pp_type == NONTERMINAL_EXPRESSION || item.pp_type == FLAG_HANDLE ) {
        list_pp_next(list);
        list_pp_get_value(list, &item);
    }

    return item;
}

bool _token_to_list_item(ParserOptions *parser_opt, TokenData token, PPListItem *item) {
    if (!_get_token_types(token, parser_opt, &item->pp_type, &item->data_type)) return false;

    item->node = malloc(sizeof item->node);
    if (item->node == NULL) {
        parser_opt->return_code = INTER_ERR;
        return false;
    }

    item->node->token = token;
    item->node->left = NULL;
    item->node->right = NULL;
    return true;
}

bool parse_check_optimize_generate_expression(ParserOptions *parser_opt) {
    PrecedenceTable pp_table = PRECEDENCE_TABLE;

    // initialization
    ListPP list;
    list_pp_init(&list);
    PPListItem empty_item = { .pp_type = TERMINAL_EMPTY };
    PPListItem handle_item = { .pp_type = FLAG_HANDLE };
    if (list_pp_insert_first(&list, empty_item) == LIST_ALLOC_ERR) {
        _free_pp_list(&list);
        return false;
    }

    // get first terminal along with its AST node
    PPListItem terminal;
    if(!_token_to_list_item(parser_opt, parser_opt->token, &terminal)) {
        _free_pp_list(&list);
        return false;
    }

    // parse and check expression, build abstract syntax tree
    while (terminal.pp_type != TERMINAL_EMPTY
           || !_list_contains_done_sequence(&list))
    {
        // look in the precedence table and perform given action
        switch (pp_table[_get_first_terminal_item(&list).pp_type][terminal.pp_type])
        {
            case PP_HANDLE_SHIFT: // <
                if (list_pp_insert_before(&list, handle_item) == LIST_ALLOC_ERR ||
                    list_pp_insert_first(&list, terminal) == LIST_ALLOC_ERR) {
                    _free_pp_list(&list);
                    return false;
                }
                break;
            case PP_REDUCE: // >
                if (!_reduce_list_until_handle(parser_opt, &list)) {
                    _free_pp_list(&list);
                    return false;
                }
                continue;
            case PP_SHIFT_REDUCE: // =
                if (list_pp_insert_first(&list, terminal) == LIST_ALLOC_ERR) {
                    _free_pp_list(&list);
                    return false;
                }
                break;
            case PP_ERROR:  // ERR
                parser_opt->return_code = STX_ERR;
                _free_pp_list(&list);
                return false;
        }

        // get next terminal along with its AST node
        _next_token(parser_opt);
        if(!_token_to_list_item(parser_opt, parser_opt->token, &terminal)) {
            _free_pp_list(&list);
            return false;
        }
    }

    // get abstract syntax tree
    PPListItem expression;
    list_pp_first(&list);
    list_pp_get_value(&list, &expression);
    ASTNode *ast = expression.node;

    // generate expression
    // TODO: generate expression from abstract syntax tree
    ast = ast;

    _free_pp_list(&list);
    return true;
}