#include "precedence_parser.h"

bool _token_to_list_item(ParserOptions *parser_opt, TokenData token, ASTNode *left, ASTNode *right, PPListItem *item) {
    // TODO: remove after usage
    parser_opt = parser_opt;

    item->type = NONTERMINAL_EXPRESSION;
    item->node.token = token;

    item->node.left = left;
    item->node.right = right;

    switch (token.type)
    {
        case TOKEN_ADD:
            item->type = TERMINAL_ADD;
            break;
        case TOKEN_SUB:
            item->type = TERMINAL_SUB;
            break;
        case TOKEN_MUL:
            item->type = TERMINAL_MUL;
            break;
        case TOKEN_DIV:
            item->type = TERMINAL_DIV;
            break;
        case TOKEN_EQUAL:
            item->type = TERMINAL_EQUAL;
            break;
        case TOKEN_NOT_EQUAL:
            item->type = TERMINAL_NOT_EQUAL;
            break;
        case TOKEN_LESSER:
            item->type = TERMINAL_LESSER;
            break;
        case TOKEN_LESSER_EQUAL:
            item->type = TERMINAL_LESSER_EQUAL;
            break;
        case TOKEN_GREATER:
            item->type = TERMINAL_GREATER;
            break;
        case TOKEN_GREATER_EQUAL:
            item->type = TERMINAL_GREATER_EQUAL;
            break;
        case TOKEN_AND:
            item->type = TERMINAL_AND;
            break;
        case TOKEN_OR:
            item->type = TERMINAL_OR;
            break;
        case TOKEN_EXCL_MARK:
            item->type = TERMINAL_EXCL_MARK;
            break;
        case TOKEN_NOT:
            item->type = TERMINAL_NOT;
            break;
        case TOKEN_NIL_COALESCING:
            item->type = TERMINAL_NIL_COALESCING;
            break;
        case TOKEN_KEYWORD_TRUE:
            item->type = TERMINAL_KEYWORD_TRUE;
            break;
        case TOKEN_KEYWORD_FALSE:
            item->type = TERMINAL_KEYWORD_FALSE;
            break;
        case TOKEN_IDENTIF:
            item->type = TERMINAL_IDENTIF;
            break;
        case TOKEN_NUMBER:
            item->type = TERMINAL_NUMBER;
            break;
        case TOKEN_STRING:
            item->type = TERMINAL_STRING;
            break;
        case TOKEN_KEYWORD_NIL:
            item->type = TERMINAL_KEYWORD_NIL;
            break;
        case TOKEN_L_BRACKET:
            item->type = TERMINAL_L_BRACKET;
            break;
        case TOKEN_R_BRACKET:
            item->type = TERMINAL_R_BRACKET;
            break;
        default:
            item->type = TERMINAL_EMPTY;
    }

    return item;
}


bool _is_simple_expression(PPListItem item) {
    return (item.type == TERMINAL_IDENTIF
           || item.type == TERMINAL_NUMBER
           || item.type == TERMINAL_STRING
           || item.type == TERMINAL_KEYWORD_NIL
           || item.type == TERMINAL_KEYWORD_TRUE
           || item.type == TERMINAL_KEYWORD_FALSE);
}

bool _is_binary_operator(PPListItem item) {
    return (item.type == TERMINAL_ADD
           || item.type == TERMINAL_SUB
           || item.type == TERMINAL_MUL
           || item.type == TERMINAL_DIV
           || item.type == TERMINAL_EQUAL
           || item.type == TERMINAL_NOT_EQUAL
           || item.type == TERMINAL_LESSER
           || item.type == TERMINAL_LESSER_EQUAL
           || item.type == TERMINAL_GREATER
           || item.type == TERMINAL_GREATER_EQUAL
           || item.type == TERMINAL_AND
           || item.type == TERMINAL_OR
           || item.type == TERMINAL_NIL_COALESCING);
}

bool _get_rule_result(ParserOptions *parser_opt, ListPP *list, PPListItem *new_item) {
    PPListItem last_item;

    // calculate the size of the right side of the rule
    int rule_right_side_size = 0;
    list_pp_first(list);
    list_pp_get_value(list, &last_item);
    while (last_item.type != FLAG_HANDLE && last_item.type != TERMINAL_EMPTY ) {
        rule_right_side_size++;
        list_pp_next(list);
        list_pp_get_value(list, &last_item);
    }

    // if no handle was found -> return failure
    if (last_item.type == TERMINAL_EMPTY) {
        parser_opt->return_code = STX_ERR;
        return false;
    }

    // if the size doesn't match with any rule -> return failure
    if (rule_right_side_size < 0 || rule_right_side_size > 3) {
        parser_opt->return_code = STX_ERR;
        return false;
    }

    // match rule and build abstract syntax tree
    list_pp_first(list);
    list_pp_get_value(list, &last_item);
    PPListItem item0;
    PPListItem item1;
    switch (rule_right_side_size)
    {
        case 1:
            // format check
            if (!_is_simple_expression(last_item)) {
                parser_opt->return_code = STX_ERR;
                return false;
            }
            // new list item along with its AST node (E)
            if (!_token_to_list_item(parser_opt, last_item.node.token, NULL,
                NULL, new_item)) return false;
            break;
        case 2:
            list_pp_next(list);
            list_pp_get_value(list, &item0);
            // format check
            if ((item0.type != NONTERMINAL_EXPRESSION
                    || last_item.type != TERMINAL_EXCL_MARK)
                && (item0.type != TERMINAL_NOT
                    || last_item.type != NONTERMINAL_EXPRESSION)) {
                parser_opt->return_code = STX_ERR;
                return false;
            }
            // new list item along with its AST node
            if (item0.type == TERMINAL_NOT) {
                // TOKEN_NOT E
                if (!_token_to_list_item(parser_opt, item0.node.token,
                    &last_item.node, NULL, new_item)) return false;
            } else {
                // E TOKEN_EXCL_MARK
                if (!_token_to_list_item(parser_opt, last_item.node.token,
                    &item0.node, NULL, new_item)) return false;
            }
            break;
        case 3:
            list_pp_next(list);
            list_pp_get_value(list, &item1);
            list_pp_next(list);
            list_pp_get_value(list, &item0);
            // format check
            if ((item0.type != TERMINAL_L_BRACKET
                    || item1.type != NONTERMINAL_EXPRESSION
                    || last_item.type != TERMINAL_R_BRACKET)
                && (item0.type != NONTERMINAL_EXPRESSION
                    || !_is_binary_operator(item1)
                    || last_item.type != NONTERMINAL_EXPRESSION)) {
                parser_opt->return_code = STX_ERR;
                return false;
            }
            // new list item along with its AST node (E OPERATOR E)
            if(!_token_to_list_item(parser_opt, item1.node.token, &item0.node,
                &last_item.node, new_item)) return false;
            break;
    }

    return true;
}

bool _apply_rule(ParserOptions *parser_opt, ListPP *list) {
    // get rule result, parse and check rule
    PPListItem new_item;
    if (!_get_rule_result(parser_opt, list, &new_item)) return false;

    // pop rule with handle
    PPListItem item;
    list_pp_get_first(list, &item);
    while (item.type != FLAG_HANDLE) {
        list_pp_delete_first(list);
        list_pp_get_first(list, &item);
    }
    list_pp_delete_first(list); // deletes handle

    // add result of the rule to the list 
    list_pp_insert_first(list, new_item);

    return true;
}

bool _list_contains_done_sequence(ListPP *list) {
    PPListItem item;

    // first item should be EXPRESSION
    list_pp_first(list);
    list_pp_get_value(list, &item);
    if (item.type != NONTERMINAL_EXPRESSION) return false;

    // second item should be bottom of the list
    list_pp_next(list);
    list_pp_get_value(list, &item);
    if (item.type != TERMINAL_EMPTY) return false;

    // third item shouldn't exist
    list_pp_next(list);
    if (list_pp_is_active(list)) return false;

    return true;
}

PPListItem _get_first_terminal(ListPP *list) {
    PPListItem item;

    list_pp_first(list);
    list_pp_get_value(list, &item);
    while (item.type == NONTERMINAL_EXPRESSION || item.type == FLAG_HANDLE ) {
        list_pp_next(list);
        list_pp_get_value(list, &item);
    }

    return item;
}

bool parse_check_optimize_generate_expression(ParserOptions *parser_opt) {
    PrecedenceTable pp_table = PRECEDENCE_TABLE;

    // initialization
    ListPP list;
    list_pp_init(&list);
    PPListItem empty_item = { .type = TERMINAL_EMPTY };
    PPListItem handle_item = { .type = FLAG_HANDLE };
    list_pp_insert_first(&list, empty_item);

    // get first terminal along with its AST node
    PPListItem terminal;
    if(!_token_to_list_item(parser_opt, parser_opt->token, NULL, NULL,
       &terminal)) {
        list_pp_dispose(&list);
        return false;
    }

    // parse and check expression, build abstract syntax tree
    while (terminal.type != TERMINAL_EMPTY
           || !_list_contains_done_sequence(&list))
    {
        // look in the precedence table and perform given action
        switch (pp_table[_get_first_terminal(&list).type][terminal.type])
        {
            case PP_HANDLE_SHIFT: // <
                list_pp_insert_before(&list, handle_item);
                list_pp_insert_first(&list, terminal);
                break;
            case PP_REDUCE: // >
                if (!_apply_rule(parser_opt, &list)) {
                    list_pp_dispose(&list);
                    return false;
                }
                continue;
            case PP_SHIFT_REDUCE: // =
                list_pp_insert_first(&list, terminal);
                break;
            case PP_ERROR: // ERR
                parser_opt->return_code = STX_ERR;
                list_pp_dispose(&list);
                return false;
        }

        // get next terminal along with its AST node
        _next_token(parser_opt);
        if(!_token_to_list_item(parser_opt, parser_opt->token, NULL, NULL,
           &terminal)) {
            list_pp_dispose(&list);
            return false;
        }
    }

    // get abstract syntax tree
    PPListItem expression;
    list_pp_first(&list);
    list_pp_get_value(&list, &expression);
    list_pp_get_value(&list, &expression);
    ASTNode ast = terminal.node;

    // generate expression
    // TODO: generate expression from abstract syntax tree
    ast = ast;
    
    list_pp_dispose(&list);
    return true;
}