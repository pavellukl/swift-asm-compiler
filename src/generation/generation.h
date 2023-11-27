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

typedef struct {
    SBuffer *selected;
    SBuffer *main;
    SBuffer *functions;
    SBuffer *scope;
    SBuffer *label;
    int scope_n;
    int if_n;
    int while_n;
    int expr_n;
    int counter_n;
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
 * @param gen_var Generation variables.
 * @return true If generation was successful.
 * @return false If an error occured.
 */
bool generate_function_end(GenerationVariables gen_var);

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
 * @param gen_var Generation variables.
 * @param symtable Pointer to the symtable.
 * @param identifier Identifier of the variable.
 * @return true If generation was successful.
 * @return false If an error occured.
 */
bool generate_variable(GenerationVariables gen_var, ListST *symtable,
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