/**
 * @name Implementation of IFJ23 language compiler
 * @file recursive_parser.h
 * @brief contains function definitions for the recursive descent and some
 * wrapper functions
 *
 * @author Pavel Lukl (xluklp00)
 * @author Jan Klanica (xklani00)
 */

#ifndef RECURSIVE_PARSER_H
#define RECURSIVE_PARSER_H

#include "../../helpers/arguments/arguments.h"
#include "../../helpers/parameters/parameters.h"
#include "../parser.h"
#include "../precedence_parser/precedence_parser.h"

/**
 * @brief this function adds inbuilt functions to the symtable
 *
 * @param symtable the symtable struct pointer
 */
bool add_inbuilt_functions_to_symtable(ListST *symtable);

/**
 * @brief this function implements the second run of the compiler
 *
 * @param parser_opt parser options struct pointer which contains data that
 * needs to be passed around throughout the compiler
 */
void parse_check_optimize_generate(ParserOptions *parser_opt);

/**
 * @brief this functions implements the first run of the compiler
 *
 * @param parser_opt parser options struct pointer which contains data that
 * needs to be passed around throughout the compiler
 */
void parse_function_definition(ParserOptions *parser_opt);

bool _program(ParserOptions *parser_opt);
bool _function_definition(ParserOptions *parser_opt);
bool _function_head(ParserOptions *parser_opt, LSTElement *func,
                    LSTElement **func_ptr);
bool __func_identif_lbracket_arglist_rbracket(ParserOptions *parser_opt,
                                              LSTElement *func);
bool _param_list(ParserOptions *parser_opt, LSTElement *func);
bool _comma_param(ParserOptions *parser_opt, LSTElement *func);
bool _param(ParserOptions *parser_opt, LSTElement *func);
bool __param_name(ParserOptions *parser_opt, Parameter *param);
bool _scope_body(ParserOptions *parser_opt);
bool _command_sequence(ParserOptions *parser_opt);
bool _command(ParserOptions *parser_opt);
bool __identif(ParserOptions *parser_opt, char *identif);
bool _data_type(ParserOptions *parser_opt, Type *type);
bool _return_command(ParserOptions *parser_opt);
bool __return(ParserOptions *parser_opt, ASTNode **expression_node);
bool _variable_def(ParserOptions *parser_opt);
bool __varlet_identif(ParserOptions *parser_opt, Type *expected_var_type,
                      ASTNode **provided_expression_node, bool *is_function);
bool __varlet_identif_colon_type(ParserOptions *parser_opt,
                                 ASTNode **provided_expression_node,
                                 bool *is_function);
bool _conditional_command(ParserOptions *parser_opt, bool *has_else_branch);
bool __if(ParserOptions *parser_opt, bool *has_else_branch);
bool __if_let_identif_body(ParserOptions *parser_opt, bool *has_else_branch);
bool __if_let_identif_body_else(ParserOptions *parser_opt,
                                bool *has_else_branch);
bool _while_command(ParserOptions *parser_opt);
bool _function_call(ParserOptions *parser_opt, Type *return_type);
bool _arg_list(ParserOptions *parser_opt, Arguments *args);
bool _comma_arg(ParserOptions *parser_opt, Arguments *args);
bool _arg(ParserOptions *parser_opt, Arguments *args);
bool __arg_name(ParserOptions *parser_opt, Argument *arg,
                bool *is_name_identifier);
bool __arg_name_colon(ParserOptions *parser_opt, Argument *arg);
bool _arg_val(ParserOptions *parser_opt, Argument *arg);

#endif /* RECURSIVE_PARSER_H */
