#include "generation.h"

bool generation_init(GenerationVariables *gen_var) {
    gen_var->main = sbuffer_init();
    if (gen_var->main == NULL) {
        return false;
    }

    gen_var->functions = sbuffer_init();
    if (gen_var->functions == NULL) {
        sbuffer_discard(gen_var->main);
        return false;
    }

    gen_var->scope = sbuffer_init();
    if (gen_var->scope == NULL) {
        sbuffer_discard(gen_var->main);
        sbuffer_discard(gen_var->functions);
        return false;
    }

    gen_var->selected = gen_var->main;
    gen_var->scope_n = 0;
    gen_var->if_n = 0;
    gen_var->while_n = 0;
    gen_var->lexpr_n = 0;

    return true;
}

void generation_free(const GenerationVariables gen_var) {
    sbuffer_discard(gen_var.main);
    sbuffer_discard(gen_var.functions);
    sbuffer_discard(gen_var.scope);
}

void print_generation_to_file(GenerationVariables gen_opt, FILE *out) {
    if (out == NULL) return;

    fprintf(out, ".IFJcode23\n\n");

    fprintf(out, "%s\n", gen_opt.main->string);
    fprintf(out, "exit\n\n");

    fprintf(out, "%s\n", gen_opt.functions->string);
}

void generate_inbuilt_functions(GenerationVariables gen_var) {
    // TODO: generate inbuilt functions
    gen_var = gen_var;
}

void _generate_string_literal(GenerationVariables *gen_var, char *str) {
    for (int i = 0; str[i] != '\0'; i++) {
        if ((str[i] >= 0 && str[i] <= 32) || str[i] == 35 || str[i] == 92) {
            sbuffer_printf(gen_var->selected, "\\0");
            if (str[i] < 10) {
                sbuffer_printf(gen_var->selected, "0");
            }
            sbuffer_printf(gen_var->selected, "%d", str[i]);
        }
    }
}

void _generate_arithmetical_expression(GenerationVariables *gen_var,
                                       ASTNode *ast, Type expr_type) {
    if (ast->left == NULL && ast->right == NULL) {
        sbuffer_printf(gen_var->selected, "PUSHS ");
        switch (ast->token.type)
        {
        case TOKEN_INT:
            if (expr_type == T_FLOAT) {
                sbuffer_printf(gen_var->selected, "float@%a",
                               (float) ast->token.value.int_value);
            } else {
                sbuffer_printf(
                    gen_var->selected, "int@%d", ast->token.value.int_value);
            }
            break;
        case TOKEN_FLOAT:
            sbuffer_printf(
                gen_var->selected, "float@%a", ast->token.value.float_value);
            break;
        case TOKEN_STRING:
            sbuffer_printf(gen_var->selected, "string@");
            _generate_string_literal(gen_var, ast->token.value.string);
            break;
        case TOKEN_BOOL:
            if (ast->token.value.boolean == true) {
                sbuffer_printf(gen_var->selected, "bool@true");
            } else {
                sbuffer_printf(gen_var->selected, "bool@false");
            }
            break;
        case TOKEN_IDENTIF:
            // TODO: generate token_identif
            break;
        case TOKEN_KEYWORD_NIL:
            sbuffer_printf(gen_var->selected, "nil@nil");
            break;
        default: // for the compiler to be happy
            break;
        }
        sbuffer_printf(gen_var->selected, "\n");
        return;
    }

    _generate_arithmetical_expression(gen_var, ast->right, expr_type);
    _generate_arithmetical_expression(gen_var, ast->left, expr_type);

    gen_var = gen_var;
    ast = ast;
}

void generate_expression(GenerationVariables *gen_var, ASTNode *ast) {
    _generate_arithmetical_expression(gen_var, ast, ast->data_type);
}