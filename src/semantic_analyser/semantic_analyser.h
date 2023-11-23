#ifndef SEMANTIC_ANALYSER_H
#define SEMANTIC_ANALYSER_H

#include <stdbool.h>
#include <string.h>

#include "../parser/parser.h"
#include "../parser/precedence_parser/ListPP/ListPP.h"
#include "../symtable/symtable.h"

/**
 * @brief initiliaze semantic context strucutre
 *
 * @param sem_ctx semantic context pointer which will be initialized
 */
void init_semantic_context(SemanticContext *sem_ctx);

/**
 * @brief analyzes function definition
 *
 * @param parser_opt parser options pointer to save possible semantic error code
 * @param params parameters pointer
 */
bool analyze_function_dec(ParserOptions *parser_opt, Parameters *params);

/**
 * @brief analyzes function call
 *
 * @param parser_opt parser options pointer to save possible semantic error code
 * @param identifier identifier of called function
 * @param arguments passed arguments
 * @param return_type pointer to type where function return type will be stored
 */
bool analyze_function_call(ParserOptions *parser_opt, char *identifier,
                           Parameters *arguments, Type *return_type);

/**
 * @brief analyzes assignment, assigns varible defined_value flag
 *
 * @param parser_opt parser options pointer to save possible semantic error code
 * @param identifier indentifier of the variable that's being assigned to
 * @param assign_type type of the assigned value
 */
bool analyze_assignment(ParserOptions *parser_opt, char *identifier,
                        Type assign_type);

/**
 * @brief analyzes return statement
 *
 * @param parser_opt parser options pointer to save possible semantic error code
 * @param fnc pointer to symtable entry of function within which to analyze
 * return
 * @param expression_type type of the expression after return
 */
bool analyze_return(ParserOptions *parser_opt, LSTElement *fnc,
                    Type expression_type);

/**
 * @brief analyzes variable definition, and adds variable to symtable
 *
 * @param parser_opt parser options pointer to save possible semantic error code
 * @param is_constant bool whether let or var was used (let = true, var = false)
 * @param identifier identifier of newly defined variable
 * @param expected_type explicitly defined type of variable (T_VOID if not
 * defined)
 * @param provided_value_type type of provided value (T_VOID if not proided)
 */
bool analyze_var_def(ParserOptions *parser_opt, bool is_constant,
                     char *identifier, Type expected_type,
                     Type provided_value_type);

/**
 * @brief analyzes if with let id syntax
 *
 * @param parser_opt parser options pointer to save possible semantic error code
 * @param identifier identifier specified after let
 * @param initial_type pointer to where the initial type of the variable will be
 * stored
 * @param found_var poiner to where the pointer of the found variable will
 * be stored
 */
bool analyze_if_let(ParserOptions *parser_opt, char *identifier,
                    Type *initial_type, LSTElement **found_var);

/**
 * @brief Tells if a list item type is an expression consisting of a single
 *        1 terminal (eg. 3, "sth", true, identifier).
 *
 * @param type Type of the list item.
 * @return true If it is a binary operator type.
 * @return false If it is not a binary operator type.
 */
bool is_simple_expression(PPListItemType pp_type);

/**
 * @brief Tells if a list item type is a binary operator type.
 *
 * @param type Type of the list item.
 * @return true If it is a binary operator type.
 * @return false If it is not a binary operator type.
 */
bool is_binary_operator(PPListItemType pp_type);

/**
 * @brief determines whether types @p l_type and @p r_type match
 * as if l_type = r_type
 *
 * @param l_type left part of "virtal type assignment", can't be T_VOID or T_NIL
 * @param r_type right part of "virtual type assignment"
 * @return true if types match
 * @return false if types don't match
 */
bool _do_types_match(Type a, Type b);

/**
 * @brief Converts type to equivalent type without possible nil value.
 *
 * @param type Type to be converted.
 * @return Converted type.
 */
Type _remove_nilable(Type type);

/**
 * @brief Tells if a value of this type is a number.
 *
 * @param type Type of the value.
 * @return true If it is a number.
 * @return false If it is not a number.
 */
bool _is_number_type(Type type);

/**
 * @brief Tells if a value of this type can possibly be nil.
 *
 * @param type Type of the value.
 * @return true If it can be nil.
 * @return false If it can't be nil.
 */
bool _is_nilable_type(Type type);

/**
 * @brief Analyzes data types of operands of a binary operation and returns
 * resulting data type.
 *
 * Ensures data types of operands are compatible with the operator.
 *
 * @param parser_opt Parser options pointer to save possible semantic error
 *                   code.
 * @param operator_type A type of the operator.
 * @param l_op_type A type of the left operand.
 * @param r_op_type A type of the right operand.
 * @param new_data_type Type pointer to save resulting data type.
 * @return true If operands' data types are ok.
 * @return false If an error occurred. See @p parser_opt.return_code.
 */
bool analyze_binary_operation(ParserOptions *parser_opt,
                              TokenType operator_type, Type l_op_type,
                              Type r_op_type, Type *new_data_type);

#endif /* SEMANTIC_ANALYSER_H */