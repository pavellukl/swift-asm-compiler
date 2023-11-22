#include "precedence_parser.h"

void _free_AST(ASTNode *node) {
    if (node == NULL) return;

    _free_AST(node->left);
    _free_AST(node->right);
    free_token(node->token);
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

bool _get_token_types(ParserOptions *parser_opt,
                      PPListItemType item_last_pp_type, TokenData token,
                      PPListItemType *pp_type, Type *data_type) {
    switch (token.type)
    {
        case TOKEN_ADD:
            *pp_type = TERMINAL_ADD;
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
            // check for special end of expression with an identifier which does
            // not fit syntactically - byt may fit outside of the expression
            if (item_last_pp_type == TERMINAL_INT
                || item_last_pp_type == TERMINAL_FLOAT
                || item_last_pp_type == TERMINAL_STRING
                || item_last_pp_type == TERMINAL_BOOL
                || item_last_pp_type == TERMINAL_IDENTIF
                || item_last_pp_type == TERMINAL_KEYWORD_NIL
                || item_last_pp_type == TERMINAL_EXCL_MARK
                || item_last_pp_type == TERMINAL_R_BRACKET) {
                *pp_type = TERMINAL_EMPTY;
                return true;
            }
            // it is identifier in the expression
            *pp_type = TERMINAL_IDENTIF;
            LSTElement *el = st_search_element(parser_opt->symtable,
                                               token.value.string, NULL);
            if (el == NULL) {
                parser_opt->return_code = UNDEFVAR_ERR;
                return false;
            }
            if (el->variant == FUNCTION) {
                parser_opt->return_code = OTHER_ERR;
                return false;
            }
            if (el->defined_value == false) {
                parser_opt->return_code = UNDEFVAR_ERR;
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
            // check if the last read token was an operator - if so we expected
            // an operand but got token which is not in expressions
            // generally -> error
            if (is_binary_operator(item_last_pp_type)
                || item_last_pp_type == TERMINAL_L_BRACKET
                || item_last_pp_type == TERMINAL_NOT) {
                parser_opt->return_code = STX_ERR;
                return false;
            }
            *pp_type = TERMINAL_EMPTY;
            return true;
    }
}


bool _build_rule_result(ParserOptions *parser_opt, PPListItem items[3],
                        int rule_r_size, PPListItem *new_item) {
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
                LSTElement *el =
                    st_search_element(parser_opt->symtable,
                                      items[2].node->token.value.string, NULL);
                if (el == NULL) {
                    parser_opt->return_code = UNDEFVAR_ERR;
                    return false;
                }
                new_item->node->data_type = el->return_type;
            } else {
                new_item->node->data_type = items[2].node->data_type;
            }
            break;
        case 2:
            if (items[1].pp_type == TERMINAL_NOT) {
                // E -> TOKEN_NOT E (items[1] items[2])
                // data type tests
                if (items[2].node->data_type != T_BOOL) {
                    parser_opt->return_code = EXPRTYPE_ERR;
                    return false;
                }

                // build new_item
                new_item->node->token = items[1].node->token;
                new_item->node->left = items[2].node;
                new_item->node->right = NULL;
                new_item->node->data_type = items[2].node->data_type;
            } else {
                // E -> E TOKEN_EXCL_MARK (items[1] items[2])
                // no data type tests (every data type is ok)
                // build new_item
                *new_item = items[1];
                _remove_nilable(new_item->node->data_type);
            }
            break;
        case 3:
            if (items[0].pp_type == TERMINAL_L_BRACKET) {
                /* E -> TOKEN_L_BRACKET E TOKEN_R_BRACKET
                   (items[0] items[1] items[2]) */
                // no data type tests (every data type is ok)
                // build new_item
                *new_item = items[1];
            } else {
                // data type tests
                if (!analyze_binary_operation(parser_opt,
                    items[1].node->token.type, items[0].node->data_type,
                    items[2].node->data_type, &new_item->node->data_type)) {
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

bool _rule_exists(PPListItem *items, int rule_r_size) {
    // if the size doesn't match with any rule -> return failure
    if (rule_r_size < 0 || rule_r_size > 3) {
        return false;
    }

    // check if a rule exists
    switch (rule_r_size)
    {
        case 1:
            if (!is_simple_expression(items[2].pp_type)) {
                return false;
            }
            break;
        case 2:
            if ((items[1].pp_type != NONTERMINAL_EXPRESSION
                    || items[2].pp_type != TERMINAL_EXCL_MARK)
                && (items[1].pp_type != TERMINAL_NOT
                    || items[2].pp_type != NONTERMINAL_EXPRESSION)) {
                return false;
            }
            break;
        case 3:
            if ((items[0].pp_type != TERMINAL_L_BRACKET
                    || items[1].pp_type != NONTERMINAL_EXPRESSION
                    || items[2].pp_type != TERMINAL_R_BRACKET)
                && (items[0].pp_type != NONTERMINAL_EXPRESSION
                    || !is_binary_operator(items[1].pp_type)
                    || items[2].pp_type != NONTERMINAL_EXPRESSION)) {
                return false;
            }
            break;
    }

    return true;
}

bool _calculate_rule_r_size(ListPP *list, int *size) {
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
        return false;
    }

    return true;
}

bool _reduce_list_until_handle(ParserOptions *parser_opt, ListPP *list) {
    // calculate the size of the right side of the rule
    int rule_r_size;
    if(!_calculate_rule_r_size(list, &rule_r_size)) {
        parser_opt->return_code = STX_ERR;
        return false;
    }

    // load all possible rule parts (max 3)
    PPListItem items[3];
    list_pp_first(list);
    list_pp_get_value(list, &items[2]);
    list_pp_next(list);
    list_pp_get_value(list, &items[1]);
    list_pp_next(list);
    list_pp_get_value(list, &items[0]);

    // check whether rule exists
    if (!_rule_exists(items, rule_r_size)) {
        parser_opt->return_code = STX_ERR;
        return false;
    }

    // get rule result
    PPListItem new_item;
    new_item.node = malloc(sizeof (*new_item.node));
    if (new_item.node == NULL) {
        parser_opt->return_code = INTER_ERR;
        return false;
    }
    if (!_build_rule_result(parser_opt, items, rule_r_size, &new_item)) {
        return false;
    }

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

bool _list_contains_done_sequence(ListPP list) {
    PPListItem item;

    // first item should be EXPRESSION
    list_pp_first(&list);
    list_pp_get_value(&list, &item);
    if (item.pp_type != NONTERMINAL_EXPRESSION) return false;

    // second item should be bottom of the list
    list_pp_next(&list);
    list_pp_get_value(&list, &item);
    if (item.pp_type != TERMINAL_EMPTY) return false;

    // third item shouldn't exist
    list_pp_next(&list);
    if (list_pp_is_active(&list)) return false;

    return true;
}

PPListItem _get_first_terminal_item(ListPP *list) {
    PPListItem item;

    list_pp_first(list);
    list_pp_get_value(list, &item);
    while (item.pp_type == NONTERMINAL_EXPRESSION
           || item.pp_type == FLAG_HANDLE ) {
        list_pp_next(list);
        list_pp_get_value(list, &item);
    }

    return item;
}

bool _token_to_pplist_item(ParserOptions *parser_opt,
                           PPListItemType item_last_pp_type, TokenData token,
                           PPListItem *item) {
    item->node = malloc(sizeof (*item->node));
    if (item->node == NULL) {
        parser_opt->return_code = INTER_ERR;
        return false;
    }

    if (!_get_token_types(parser_opt, item_last_pp_type, token, &item->pp_type,
                          &item->node->data_type)) {
        return false;
    }

    if(!clone_token(&item->node->token, token)) {
        parser_opt->return_code = INTER_ERR;
        return false;
    }
    item->node->left = NULL;
    item->node->right = NULL;
    return true;
}

bool parse_check_optimize_generate_expression(ParserOptions *parser_opt) {
    PrecedenceTable pp_table = PRECEDENCE_TABLE;
    PRINTF_STDDEBUG("expression syntax/semantic checking\n")

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
    PPListItemType prev_terminal_pp_type = TERMINAL_ADD; /* expecting at least
    1 operand - nonempty expression */
    PPListItem terminal;
    if(!_token_to_pplist_item(parser_opt, prev_terminal_pp_type,
                              parser_opt->token, &terminal)) {
        _free_pp_list(&list);
        return false;
    }

    // parse and check expression, build abstract syntax tree
    while (terminal.pp_type != TERMINAL_EMPTY
           || !_list_contains_done_sequence(list))
    {
        // look in the precedence table and perform given action
        switch (
            pp_table[_get_first_terminal_item(&list).pp_type][terminal.pp_type])
        {
            case PP_HANDLE_SHIFT: // <
                if (list_pp_insert_before(&list, handle_item) == LIST_ALLOC_ERR 
                   || list_pp_insert_first(&list, terminal) == LIST_ALLOC_ERR) {
                    _free_pp_list(&list);
                    return false;
                }
                prev_terminal_pp_type = terminal.pp_type;
                _next_token(parser_opt);
                if(!_token_to_pplist_item(parser_opt, prev_terminal_pp_type,
                                          parser_opt->token, &terminal)) {
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
                if (!_reduce_list_until_handle(parser_opt, &list)) {
                    _free_pp_list(&list);
                    return false;
                }
                prev_terminal_pp_type = terminal.pp_type;
                _next_token(parser_opt);
                if(!_token_to_pplist_item(parser_opt, prev_terminal_pp_type,
                                          parser_opt->token, &terminal)) {
                    _free_pp_list(&list);
                    return false;
                }
                break;
            case PP_ERROR:  // ERR
                parser_opt->return_code = STX_ERR;
                _free_pp_list(&list);
                return false;
        }
    }

    // get abstract syntax tree
    PPListItem expression;
    list_pp_first(&list);
    list_pp_get_value(&list, &expression);
    ASTNode *ast = expression.node;
    parser_opt->variables.type = expression.node->data_type;

    // generate expression
    PRINTF_STDDEBUG("expression generation\n")
    if (!generate_expression(&parser_opt->gen_var, ast)) {
        parser_opt->return_code = INTER_ERR;
        return false;
    }

    PRINTF_STDDEBUG("expression cleanup\n")
    _free_pp_list(&list);
    PRINTF_STDDEBUG("expression done\n")
    return true;
}