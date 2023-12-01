/**
 * @file generation.h
 * @brief Contains definitions of structs and functions for the generation.
 *
 * @author Jan Klanica (xklani00)
 * @author Denis Milistenfer (xmilis00)
 * @author Pavel Lukl (xluklp00)
 */

#ifndef GENERATION_H
#define GENERATION_H

#include <stdio.h>
#include <stdbool.h>
#include "../helpers/string_buffer/string_buffer.h"
#include "../scanner/token.h"
#include "../symtable/ListST/ListST.h"
#include "../symtable/symtable.h"
#include "../helpers/utils.h"
#include "../debug.h"

/** @brief Handy struct for passing variables in recursive descent parser */
typedef struct {
    SBuffer *selected; /* main or functions */
    SBuffer *main; /* buffer for main scope */
    SBuffer *functions; /* buffer for functions scope */
    SBuffer *scope; /* buffer for side saving content of the scope */
    SBuffer *label; /* buffer for default label name */
    int scope_n; /* counter of scopes */
    int if_n; /* counter of if occurrence */
    int while_n; /* counter of while occurrence */
    int expr_n; /* counter of expression occurrence */
    int counter_n; /* simple counter for expression */
} GenerationVariables;

/** @brief Abstract syntax tree node for an expression. */
typedef struct ASTNode {
    TokenData token;
    Type data_type;
    struct ASTNode *left;
    struct ASTNode *right;
} ASTNode;

/**
 * @brief Initializes generation variables.
 * 
 * @param gen_opt 
 * @return true If initialization was successful.
 * @return false If an error occured.
 */
bool generation_init(GenerationVariables *gen_opt);

/**
 * @brief Unallocates memory space used by generation variables.
 * 
 * @param gen_opt 
 */
void generation_free(const GenerationVariables gen_opt);

/**
 * @brief Writes content of the generation buffers to the file.
 * 
 * @param gen_var Pointer to the generation variables.
 * @param out Pointer to the output file.
 */
void print_generation_to_file(GenerationVariables gen_opt, FILE *out);

/**
 * @brief Generates a function label and creating new frame with data loading
 *        into variables.
 * 
 * @param gen_var Generation variables.
 * @param fn Function which beginning should be generated.j
 * @return true If generation was successful.
 * @return false If an error occured.
 */
bool generate_function_beginning(GenerationVariables gen_var, LSTElement *fn);

/**
 * @brief Generates function end.
 * 
 * @param sbuffer Where to generate function end.
 * @return true If generation was successful.
 * @return false If an error occured.
 */
bool generate_function_end(SBuffer *sbuffer);

/**
 * @brief Generates inbuilt functions.
 * 
 * @param symtable Pointer to the symtable.
 * @param gen_var Pointer to the generation variables.
 * @return true If generation was successful.
 * @return false If an error occured.
 */
bool generate_inbuilt_functions(GenerationVariables gen_var, ListST *symtable);

/**
 * @brief Generates assignment instruction.
 * 
 * @param gen_var Generation variables.
 * @param symtable Pointer to the symtable.
 * @param assign_to_identif Identifier of the variable to which the value is
 *                          assigned.
 * @return true If generation was successful.
 * @return false If an error occured.
 */
bool generate_assignment(GenerationVariables gen_var, ListST *symtable,
                         char *assign_to_identif);

/**
 * @brief Generates function call instruction.
 *
 * @param gen_var Generation variables.
 * @param identifier Identifier of the called function.
 * @return true If generation was successful.
 * @return false If an error occured.
 */
bool generate_fnc_call(GenerationVariables gen_var, char *identifier);

/**
 * @brief Generates variable definition.
 * 
 * @param gen_var Pointer to the generation variables.
 * @param symtable Pointer to the symtable.
 * @param identifier Identifier of the variable.
 * @param provided_expression_node Expression node
 *                                 (type is T_VOID if no expression)
 * @param is_function Whether the assigned value is a function.
 * @return true If generation was successful.
 * @return false If an error occured.
 */
bool generate_variable_definition(GenerationVariables *gen_var,
                                  ListST *symtable, char *identifier,
                                  Type expected_var_type,
                                  ASTNode *provided_expression_node,
                                  bool is_function);

/**
 * @brief Generates function call argument.
 *
 * @param gen_var Generation variables.
 * @param symtable Pointer to the symtable.
 * @param arg Argument structure of the argument.
 * @param expected_type Expected type of the argument (type of the matching
 * parameter)
 * @return true If generation was successful.
 * @return false If an error occured.
 */
bool generate_argument(GenerationVariables gen_var, ListST *symtable,
                       Argument arg, Type expected_type);

/**
 * @brief Generates a variable. eg. LF@8var
 * 
 * @param sbuffer Where to generate a variable.
 * @param symtable Pointer to the symtable.
 * @param identifier Identifier of the variable.
 * @return true If generation was successful.
 * @return false If an error occured.
 */
bool generate_variable(SBuffer *sbuffer, ListST *symtable,
                        char *identifier);

/**
 * @brief Generates string literal.
 * 
 * @param gen_var Generation variables.
 * @param str String to be generated.
 * @return true If generation was successful.
 * @return false If an error occured.
 */
bool generate_string_literal(GenerationVariables gen_var, char *str);

/**
 * @brief Generates a literal.
 * 
 * @param gen_var Generation variables.
 * @param token Token containing literal to be generated.
 * @param expected_type Expected type of the generated literal.
 * @return true If generation was successful.
 * @return false If an error occured.
 */
bool generate_literal(GenerationVariables gen_var, TokenData token,
                      Type expected_type);

/**
 * @brief Generates expression. Does not know AND, OR, NOT.
 * 
 * @param gen_var Pointer to the generation variables.
 * @param ast Abstract syntax tree representing expression.
 * @param symtable Pointer to the symtable.
 * @param expr_type Resulting expression type.
 * @return true If generation was successful.
 * @return false If an error occured.
 */
bool _generate_simply_expression(GenerationVariables *gen_var, ASTNode *ast,
                                 ListST *symtable, Type expr_type);

/**
 * @brief Generates an expression to use short circuit evaluation.
 * 
 * @param gen_var Pointer to the generation variables.
 * @param ast Abstract syntax tree representing expression.
 * @param symtable Pointer to the symtable.
 * @param t Suffix of the label in case of true.
 * @param f Suffix of the label in case of false.
 * @return true If generation was successful.
 * @return false If an error occured.
 */
bool _generate_short_circuit_eval(GenerationVariables *gen_var, ASTNode *ast,
                                  ListST *symtable, int t, int f);

/**
 * @brief Creates new empty ast node.
 * 
 * @return ASTNode* New ast node or NULL if malloc error occured.
 */
ASTNode* ast_init();

/**
 * @brief Generates an expression.
 * 
 * @param gen_var Pointer to the generation variables.
 * @param ast Abstract syntax tree representing expression.
 * @param symtable Pointer to the symtable.
 * @return true If generation was successful.
 * @return false If an error occured.
 */
bool generate_expression(GenerationVariables *gen_var, ASTNode *ast,
                         ListST *symtable);

#endif /* GENERATION_H */