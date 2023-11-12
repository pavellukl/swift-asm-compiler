#ifndef PRECEDENCE_PARSER_H
#define PRECEDENCE_PARSER_H

// #include "../scanner/scanner.h"
// #include "../helpers/list/list.h"
#include "../parser.h"
#include "../../helpers/list/ListPP/ListPP.h"
#include "precedence_table.h"

bool parse_check_optimize_generate_expression(ParserOptions *parser_opt);

PPListItem _token_to_list_item(TokenData token);
bool _list_contains_done_sequence(ListPP *list);
PPListItem _get_first_terminal(ListPP *list);
bool _is_simple_expression(PPListItem item);
bool _is_binary_operator(PPListItem item);
bool _match_rule(ParserOptions *parser_opt, ListPP *list);
bool _apply_rule(ParserOptions *parser_opt, ListPP *list);

#endif /* PRECEDENCE_PARSER_H */