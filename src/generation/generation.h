#ifndef GENERATION_H
#define GENERATION_H

#include <stdio.h>
#include <stdbool.h>
#include "../helpers/string_buffer/string_buffer.h"
#include "../scanner/token.h"

typedef struct {
    SBuffer *selected;
    SBuffer *main;
    SBuffer *functions;
    SBuffer *scope;
    int scope_n;
    int if_n;
    int while_n;
    int lexpr_n;
} GenerationVariables;

/** @brief Abstract syntax tree node for expression. */
typedef struct ASTNode {
    TokenData token;
    struct ASTNode *left;
    struct ASTNode *right;
} ASTNode;

bool generation_init(GenerationVariables *gen_opt);
void generation_free(const GenerationVariables gen_opt);
void print_generation_to_file(GenerationVariables gen_opt, FILE *out);

void generate_inbuilt_functions(GenerationVariables gen_var);
void _generate_arithmetical_expression(GenerationVariables *gen_var,
                                       ASTNode *ast);
void generate_expression(GenerationVariables *gen_var, ASTNode *ast);

#endif /* GENERATION_H */