#ifndef PRECEDENCE_PARSER_FUNC_H
#define PRECEDENCE_PARSER_FUNC_H

#include "parser.h"
#include "precedence_parser_types.h"
#include "../helpers/list/list.h"

bool parse_check_optimize_generate_expression(ParserOptions *parser_opt);

/**
 * @brief Converts token type (enum) to terminal (enum).
 * 
 * @param token_type Token to be converted.
 * @param terminal Pointer to variable where conversion result is stored.
 * @return true If conversion exists.
 * @return false If conversion doesn't exist.
 */
bool _token_type_to_terminal(TokenType token_type, PPListItemType *terminal);

#endif /* PRECEDENCE_PARSER_FUNC_H */