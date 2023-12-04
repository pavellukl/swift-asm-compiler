/**
 * @name Implementation of IFJ23 language compiler
 * @file precedence_parser.h
 * @brief Contains definitions of structs and functions for the precedence
 *        parser.
 *
 * @author Jan Klanica (xklani00)
 */

#ifndef PRECEDENCE_PARSER_H
#define PRECEDENCE_PARSER_H

#include "../parser.h"
#include "ListPP/ListPP.h"
#include "precedence_table.h"
#include "../../generation/generation.h"
#include "../../debug.h"

/**
 * @brief Performs syntax check, semantic check and generating upon expression.
 *
 * @param parser_opt Parser options pointer to save possible semantic error
 *                   code.
 * @param ast Pointer to the pointer to the abstract syntax tree representing an
 * expression. Parsed expression is stored here.
 *
 * @return true If the operation was successful.
 * @return false If an error occurred. See @p parser_opt.return_code.
 */
bool parse_check_optimize_expression(ParserOptions *parser_opt, ASTNode **ast);

/**
 * @brief Releases all memory from AST node and it's left and right children.
 *
 * @param node Pointer to the abstract syntax tree node.
 */
void _free_AST(ASTNode *node);

/**
 * @brief Removes all items from the precedence parsing list while
 *        unallocating their memory space as well.
 *
 * @param list Pointer to a list to operate upon.
 */
void _free_pp_list(ListPP *list);

/**
 * @brief Derives, if possible, precedence parsing type and data type from
 *        a token.
 *
 * @param parser_opt Parser options pointer to save possible semantic error
 *                   code.
 * @param token Derives types from this token.
 * @param item_last_pp_type PP type of the last item read from input.
 * @param pp_type Precedence parsing type pointer for result type.
 * @param data_type Data type pointer for result type.
 * @param dtype_is_convertable Whether is data type convertable.
 * @return true If the operation was successful.
 * @return false If an error occurred. See @p parser_opt.return_code.
 */
bool _get_token_types(ParserOptions *parser_opt,
                      PPListItemType item_last_pp_type, TokenData token,
                      PPListItemType *pp_type, Type *data_type,
                      bool *dtype_is_convertable);

/**
 * @brief Derives rule result. Performs rule semantic analysis.
 *
 * @param parser_opt Parser options pointer to save possible semantic error
 *                   code.
 * @param items Array of 3 items - l_operand operator r_operand
 * @param rule_r_size Size of the right side of the rule.
 * @param new_item Pointer to a variable for result new item.
 * @return true If the operation was successful.
 * @return false If an error occurred. See @p parser_opt.return_code.
 */
bool _build_rule_result(ParserOptions *parser_opt, PPListItem items[3],
                        int rule_r_size, PPListItem *new_item);

/**
 * @brief Tells if the first rule in the list of @p rule_r_size size
 *        exists. 
 *
 * @param list Pointer to a list to operate upon.
 * @param rule_r_size Size of the right side of the rule.
 * @return true If rule exists.
 * @return false If rule does not exist.
 */
bool _rule_exists(PPListItem *items, int rule_r_size);

/**
 * @brief Calculates how many symbols are on the right in the rule which is the
 *        first in the list.  
 *
 * @param list Pointer to a list to operate upon.
 * @param size Pointer to a variable for result.
 * @return true If operation was successful.
 * @return false If rule does not end with the handle.
 */
bool _calculate_rule_r_size(ListPP *list, int *size);

/**
 * @brief Performs one rule according to grammer.
 *
 * @param parser_opt Parser options pointer to save possible semantic error
 *                   code.
 * @param list Pointer to a list to operate upon.
 * @return true If the operation was successful.
 * @return false If an error occurred. See @p parser_opt.return_code.
 */
bool _reduce_list_until_handle(ParserOptions *parser_opt, ListPP *list);

/**
 * @brief Tells if a list contains only NONTERMINAL_EXPRESSION and FLAG_EMPTY.
 *
 * @param list List to operate upon.
 * @return true If list contains only done sequence.
 * @return false If list does not contain only done sequence.
 */
bool _list_contains_done_sequence(ListPP list);

/**
 * @brief Searches list from top to bottom and returns first terminal it founds.
 *        Note that it assumes that the list contains a terminal item.
 *
 * @param list Pointer to the List to search in.
 * @return First terminal in the list.
 */
PPListItem _get_first_terminal_item(ListPP *list);

/**
 * @brief Converts a token to the ListPP item.
 *
 * @param parser_opt Parser options pointer to save possible semantic error
 *                   code.
 * @param item_last_pp_type PP type of the last item read from input.
 * @param token Token to be converted.
 * @param item Poiner to a variable for the result item.
 * @return true If the operation was successful.
 * @return false If an error occurred. See @p parser_opt.return_code.
 */
bool _token_to_pplist_item(ParserOptions *parser_opt,
                           PPListItemType item_last_pp_type, TokenData token,
                           PPListItem *item);

#endif /* PRECEDENCE_PARSER_H */