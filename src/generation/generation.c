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
    // TODO:
    gen_var = gen_var;
}

void _generate_arithmetical_expression(GenerationVariables *gen_var,
                                       ASTNode *ast) {
    if (ast->left == NULL && ast->right == NULL) {
        // TODO: print int, float, string, Bool, Nil
        sbuffer_printf(gen_var->selected, "\n");
        return;
    }


    gen_var = gen_var;
    ast = ast;
}

void generate_expression(GenerationVariables *gen_var, ASTNode *ast) {
    gen_var = gen_var;
    ast = ast;
}