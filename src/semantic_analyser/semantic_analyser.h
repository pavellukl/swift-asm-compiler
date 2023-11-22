#ifndef SEMANTIC_ANALYSER_H
#define SEMANTIC_ANALYSER_H

#include <stdbool.h>
#include <string.h>

#include "../parser/parser.h"
#include "../parser/precedence_parser/ListPP/ListPP.h"
#include "../symtable/symtable.h"

#define PARAM_ARR_INC_N_ITEMS 4

/**
 * @brief analyzes function definition
 *
 * @param parser_opt parser options pointer to save possible semantic error code
 */
bool analyze_function_dec(ParserOptions *parser_opt);

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
 * @param fnc symtable entry of function within which
 * @param expression_type type of the expression after return
 */
bool analyze_return(ParserOptions *parser_opt, LSTElement fnc,
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
 */
bool analyze_if_let(ParserOptions *parser_opt, char *identifier,
                    Type *initial_type);

/**
 * @brief initialize helper parameter array variable, beware memory leak
 *
 * @param params pointer to the parameters struct which's array will be
 * initialized
 */
void init_parameter_array(Parameters *params);

/**
 * @brief add a parameter into parameter array helper
 *
 * @param params pointer to the parameters struct into which the parameter will
 * be added
 * @return true if the operation was successful
 * @return false if the operation was not successful
 */
bool add_to_parameter_array(Parameters *params, Parameter param);

/**
 * @brief invalidates all values in the array, making it work like it's empty
 *
 * @param params pointer to the parameters struct which's array will be
 * invalidated
 */
void invalidate_parameter_array(Parameters *params);

/**
 * @brief unallocates parameter array and reinitiliazes the structure
 *
 * @param params pointer to the parameters struct which's array will be
 * unallocated
 */
void destroy_parameter_array(Parameters *params);

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