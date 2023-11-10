#include "precedence_parser_func.h"

bool _token_type_to_terminal(TokenType token_type, PPListItemType *terminal) {
    switch (token_type)
    {
        case TOKEN_ADD: *terminal = TERMINAL_ADD; break;
        case TOKEN_SUB: *terminal = TERMINAL_SUB; break;
        case TOKEN_MUL: *terminal = TERMINAL_MUL; break;
        case TOKEN_DIV: *terminal = TERMINAL_DIV; break;
        case TOKEN_EQUAL: *terminal = TERMINAL_EQUAL; break;
        case TOKEN_NOT_EQUAL: *terminal = TERMINAL_NOT_EQUAL; break;
        case TOKEN_LESSER: *terminal = TERMINAL_LESSER; break;
        case TOKEN_LESSER_EQUAL: *terminal = TERMINAL_LESSER_EQUAL; break;
        case TOKEN_GREATER: *terminal = TERMINAL_GREATER; break;
        case TOKEN_GREATER_EQUAL: *terminal = TERMINAL_GREATER_EQUAL; break;
        case TOKEN_AND: *terminal = TERMINAL_AND; break;
        case TOKEN_OR: *terminal = TERMINAL_OR; break;
        case TOKEN_EXCL_MARK: *terminal = TERMINAL_EXCL_MARK; break;
        case TOKEN_NOT: *terminal = TERMINAL_NOT; break;
        case TOKEN_NIL_COALESCING: *terminal = TERMINAL_NIL_COALESCING; break;
        case TOKEN_KEYWORD_TRUE: *terminal = TERMINAL_KEYWORD_TRUE; break;
        case TOKEN_KEYWORD_FALSE: *terminal = TERMINAL_KEYWORD_FALSE; break;
        case TOKEN_IDENTIF: *terminal = TERMINAL_IDENTIF; break;
        case TOKEN_NUMBER: *terminal = TERMINAL_NUMBER; break;
        case TOKEN_STRING: *terminal = TERMINAL_STRING; break;
        case TOKEN_KEYWORD_NIL: *terminal = TERMINAL_KEYWORD_NIL; break;
        case TOKEN_L_BRACKET: *terminal = TERMINAL_L_BRACKET; break;
        case TOKEN_R_BRACKET: *terminal = TERMINAL_R_BRACKET; break;
        default: return false;
    }
    return true;
}

bool parse_check_optimize_generate_expression(ParserOptions *parser_opt) {
    parser_opt->return_code = parser_opt->return_code;

    // PrecedenceTable pp_table = PRECEDENCE_TABLE;
    ListPP list;
    list_pp_init(&list);

    return true;
}