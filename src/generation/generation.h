#ifndef GENERATION_H
#define GENERATION_H

#include <stdio.h>
#include <stdbool.h>
#include "../helpers/string_buffer/string_buffer.h"
#include "../scanner/token.h"
#include "../symtable/ListST/ListST.h"
#include "../symtable/symtable.h"
#include "../helpers/utils.h"

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

/** @brief Abstract syntax tree node for expression. */
typedef struct ASTNode {
    TokenData token;
    Type data_type;
    struct ASTNode *left;
    struct ASTNode *right;
} ASTNode;

bool generation_init(GenerationVariables *gen_opt);
void generation_free(const GenerationVariables gen_opt);
void print_generation_to_file(GenerationVariables gen_opt, FILE *out);

bool generate_function_start(GenerationVariables gen_var, LSTElement *fn);
bool generate_inbuilt_functions(ListST *symtable, GenerationVariables gen_var);

bool _generate_string_literal(GenerationVariables *gen_var, char *str);
bool _generate_arithmetical_expression(GenerationVariables *gen_var,
                                       ASTNode *ast, Type expr_type);
bool _generate_logical_expression(GenerationVariables *gen_var, ASTNode *ast,
                                  int t, int f);
bool generate_expression(GenerationVariables *gen_var, ASTNode *ast);

#endif /* GENERATION_H */