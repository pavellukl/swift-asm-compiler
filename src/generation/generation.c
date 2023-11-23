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

    gen_var->label = sbuffer_init();
    if (gen_var->label == NULL) {
        sbuffer_discard(gen_var->main);
        sbuffer_discard(gen_var->functions);
        sbuffer_discard(gen_var->scope);
        return false;
    }

    gen_var->selected = gen_var->main;
    gen_var->scope_n = 0;
    gen_var->if_n = 0;
    gen_var->while_n = 0;
    gen_var->expr_n = 0;

    return true;
}

void generation_free(const GenerationVariables gen_var) {
    sbuffer_discard(gen_var.main);
    sbuffer_discard(gen_var.functions);
    sbuffer_discard(gen_var.scope);
    sbuffer_discard(gen_var.label);
}

void print_generation_to_file(GenerationVariables gen_opt, FILE *out) {
    if (out == NULL) return;

    fprintf(out, ".IFJcode23\n\n");

    fprintf(out, "%s", gen_opt.main->string);

    if (gen_opt.functions->size > 0) {
        fprintf(out, "  exit int@0\n");
        fprintf(out, "%s\n", gen_opt.functions->string);
    }
}

bool generate_function_start(GenerationVariables gen_var, LSTElement *fn) {
    if (fn->variant != FUNCTION) return false;
    if (fn->value.parameters.infinite) return false;

    SBUFFER_PRINTF(gen_var.selected, "\nLABEL _%s\n"
                                       "  CREATEFRAME\n", fn->identifier);

    for (int i = fn->value.parameters.size - 1; i >= 0; i--) {
        SBUFFER_PRINTF(gen_var.selected, "  DEFVAR TF@%s\n"
                                         "  PUSHS TF@%s\n",
                                        fn->value.parameters.parameters_arr[i],
                                        fn->value.parameters.parameters_arr[i]);
    }

    SBUFFER_PRINTF(gen_var.selected, "  PUSHFRAME\n");

    return true;
}

bool generate_inbuilt_functions(ListST *symtable, GenerationVariables gen_var) {
    SBuffer *init_buffer = gen_var.selected;

    gen_var.selected = gen_var.functions;
    int scope;
    LSTElement *el;

    el = st_search_element(symtable, "readString", &scope);
    if (el == NULL || !generate_function_start(gen_var, el)) return false;
    SBUFFER_PRINTF(gen_var.selected, "  READ LF@result string\n"
                                     "  PUSHS LF@result\n"
                                     "  RETURN\n");

    el = st_search_element(symtable, "readInt", &scope);
    if (el == NULL || !generate_function_start(gen_var, el)) return false;
    SBUFFER_PRINTF(gen_var.selected, "  READ LF@result int\n"
                                     "  PUSHS LF@result\n"
                                     "  RETURN\n");

    el = st_search_element(symtable, "readDouble", &scope);
    if (el == NULL || !generate_function_start(gen_var, el)) return false;
    SBUFFER_PRINTF(gen_var.selected, "  READ LF@result double\n"
                                     "  PUSHS LF@result\n"
                                     "  RETURN\n");

    el = st_search_element(symtable, "Int2Double", &scope);
    if (el == NULL || !generate_function_start(gen_var, el)) return false;
    SBUFFER_PRINTF(gen_var.selected, "  INT2FLOAT LF@result LF@term\n"
                                     "  PUSHS LF@result\n"
                                     "  RETURN\n");

    el = st_search_element(symtable, "Double2Int", &scope);
    if (el == NULL || !generate_function_start(gen_var, el)) return false;
    SBUFFER_PRINTF(gen_var.selected, "  FLOAT2INT LF@result LF@term\n"
                                     "  PUSHS LF@result\n"
                                     "  RETURN\n");

    el = st_search_element(symtable, "length", &scope);
    if (el == NULL || !generate_function_start(gen_var, el)) return false;
    SBUFFER_PRINTF(gen_var.selected, "  STRLEN LF@result LF@s\n"
                                     "  PUSHS LF@result\n"
                                     "  RETURN\n");

    el = st_search_element(symtable, "substring", &scope);
    if (el == NULL || !generate_function_start(gen_var, el)) return false;
    
    //conditions returning nil
    SBUFFER_PRINTF(gen_var.selected, "  DEFVAR LF@cond\n"
                                     "  LT LF@cond LF@i int@0\n"                          //i < 0
                                     "  JUMPIFEQ _string_return_nil LF@cond bool@true\n"
                                     "  LT LF@cond LF@j int@0\n"                          //j < 0
                                     "  JUMPIFEQ _string_return_nil LF@cond bool@true\n"
                                     "  GT LF@cond LF@i LF@j\n"                           //i > j
                                     "  JUMPIFEQ _string_return_nil LF@cond bool@true\n"
                                     "  STRLEN LF@length LF@s\n"
                                     "  DEFVAR LF@cond2\n"
                                     "  GT LF@cond LF@i LF@length\n"                      //i >= length(s)
                                     "  EQ LF@cond2 LF@i LF@length\n"
                                     "  OR LF@cond LF@cond LF@cond2\n"
                                     "  JUMPIFEQ _string_return_nil LF@cond bool@true\n"
                                     "  GT LF@cond LF@j LF@length\n"                      //j > length(s)
                                     "  JUMPIFEQ _string_return_nil LF@cond bool@true\n");
    //getting substring
    SBUFFER_PRINTF(gen_var.selected, "  DEFVAR LF@substring\n"
                                     "  MOVE LF@substring string@\n"
                                     "  DEFVAR LF@string_index\n"
                                     "  MOVE LF@string_index LF@i\n"
                                     "  DEFVAR LF@char\n"
                                     "\nLABEL _string_loop_start\n"
                                     "  JUMPIFEQ _string_loop_end LF@string_index LF@j\n"
                                     "  GETCHAR LF@char LF@s LF@string_index\n"
                                     "  CONCAT LF@substring LF@substring LF@char\n"
                                     "  ADD LF@string_index LF@string_index int@1\n"
                                     "  JUMP _string_loop_start\n"
                                     "\nLABEL _string_loop_end\n");
    //return
    SBUFFER_PRINTF(gen_var.selected, "  PUSHS LF@substring\n"
                                     "  RETURN\n");
    //nil return
    SBUFFER_PRINTF(gen_var.selected, "\nLABEL _string_return_nil\n"
                                     "  PUSHS nil@nil\n"
                                     "  RETURN\n");

    el = st_search_element(symtable, "ord", &scope);
    if (el == NULL || !generate_function_start(gen_var, el)) return false;
    SBUFFER_PRINTF(gen_var.selected, "  STRI2INT LF@result LF@c int@0\n"
                                     "  PUSHS LF@result\n"
                                     "  RETURN\n");

    el = st_search_element(symtable, "chr", &scope);
    if (el == NULL || !generate_function_start(gen_var, el)) return false;
    SBUFFER_PRINTF(gen_var.selected, "  INT2CHAR LF@result LF@i\n"
                                     "  PUSHS LF@result\n"
                                     "  RETURN\n");

    el = st_search_element(symtable, "write", &scope);
    if (el == NULL) {
         return false;
    }
    SBUFFER_PRINTF(gen_var.selected, "\nLABEL _write\n"
                                     "  CREATEFRAME\n"
                                     "  PUSHFRAME\n");

    //pop the parameter count
    SBUFFER_PRINTF(gen_var.selected, "  DEFVAR LF@param_count\n"
                                     "  POPS LF@param_count\n");

    //printing parameters
    SBUFFER_PRINTF(gen_var.selected, "  DEFVAR LF@counter\n"
                                     "  MOVE LF@counter int@0\n"
                                     "LABEL _write_loop_start\n"
                                     "  JUMPIFEQ _write_loop_end LF@counter LF@param_count\n"
                                     "  DEFVAR LF@temp_var\n"
                                     "  POPS LF@temp_var\n"
                                     "  WRITE LF@temp_var\n"
                                     "  ADD LF@counter LF@counter int@1\n"
                                     "  JUMP _write_loop_start\n"
                                     "LABEL _write_loop_end\n");

    //return
    SBUFFER_PRINTF(gen_var.selected, "  RETURN\n");

    gen_var.selected = init_buffer;
    return true;
}

bool _generate_string_literal(GenerationVariables *gen_var, char *str) {
    for (int i = 0; str[i] != '\0'; i++) {
        if ((str[i] >= 0 && str[i] <= 32) || str[i] == 35 || str[i] == 92) {
            SBUFFER_PRINTF(gen_var->selected, "\\0");
            if (str[i] < 10) {
                SBUFFER_PRINTF(gen_var->selected, "0");
            }
            SBUFFER_PRINTF(gen_var->selected, "%d", str[i]);
        } else {
            SBUFFER_PRINTF(gen_var->selected, "%c", str[i]);
        }
    }
    return true;
}

bool _generate_arithmetical_expression(GenerationVariables *gen_var,
                                       ASTNode *ast, Type expr_type) {
    if (ast->left == NULL && ast->right == NULL) {
        SBUFFER_PRINTF(gen_var->selected, "  PUSHS ");
        switch (ast->token.type)
        {
        case TOKEN_INT:
            if (expr_type == T_FLOAT) {
                SBUFFER_PRINTF(gen_var->selected, "float@%a\n",
                               (float) ast->token.value.int_value);
            } else {
                SBUFFER_PRINTF(
                    gen_var->selected, "int@%d\n", ast->token.value.int_value);
            }
            return true;
        case TOKEN_FLOAT:
            SBUFFER_PRINTF(
                gen_var->selected, "float@%a\n", ast->token.value.float_value);
            return true;
        case TOKEN_STRING:
            SBUFFER_PRINTF(gen_var->selected, "string@");
            if (!_generate_string_literal(gen_var, ast->token.value.string))
                return false;
            SBUFFER_PRINTF(gen_var->selected, "\n");
            return true;
        case TOKEN_BOOL:
            if (ast->token.value.boolean == true) {
                SBUFFER_PRINTF(gen_var->selected, "bool@true\n");
            } else {
                SBUFFER_PRINTF(gen_var->selected, "bool@false\n");
            }
            return true;
        case TOKEN_IDENTIF:
            // TODO: generate token_identif
            // ast->data_type has identifiers data type without nilable part
            // (as opposed to symtable)
            return true;
        case TOKEN_KEYWORD_NIL:
            SBUFFER_PRINTF(gen_var->selected, "nil@nil\n");
            return true;
        default: // for the compiler to be happy
            return false;
        }
    }

    if (!_generate_arithmetical_expression(gen_var, ast->right, expr_type))
        return false;
    if (!_generate_arithmetical_expression(gen_var, ast->left, expr_type))
        return false;

    switch (ast->token.type)
    {
    case TOKEN_ADD:
        if (ast->data_type == T_STRING) {
            SBUFFER_PRINTF(gen_var->selected,
                                        "  POPS TF@temp1\n"
                                        "  POPS TF@temp0\n"
                                        "  CONCAT TF@temp0 TF@temp0 TF@temp1\n"
                                        "  PUSHS TF@temp0");
        } else {
            SBUFFER_PRINTF(gen_var->selected, "  ADDS\n");
        }
        return true;
    case TOKEN_SUB:
        SBUFFER_PRINTF(gen_var->selected, "  SUBS\n");
        return true;
    case TOKEN_MUL:
        SBUFFER_PRINTF(gen_var->selected, "  MULS\n");
        return true;
    case TOKEN_DIV:
        if (ast->left->data_type == T_INT) {
            SBUFFER_PRINTF(gen_var->selected, "  IDIVS\n");
        } else {
            SBUFFER_PRINTF(gen_var->selected, "  DIVS\n");
        }
        return true;
    case TOKEN_EQUAL:
        SBUFFER_PRINTF(gen_var->selected, "  EQS\n");
        return true;
    case TOKEN_NOT_EQUAL:
        SBUFFER_PRINTF(gen_var->selected, "  EQS\n"
                                          "  NOTS\n");
        return true;
    case TOKEN_LESSER:
        SBUFFER_PRINTF(gen_var->selected, "  LTS\n");
        return true;
    case TOKEN_LESSER_EQUAL:
        SBUFFER_PRINTF(gen_var->selected, "  GTS\n"
                                          "  NOTS\n");
        return true;
    case TOKEN_GREATER:
        SBUFFER_PRINTF(gen_var->selected, "  GTS\n");
        return true;
    case TOKEN_GREATER_EQUAL:
        SBUFFER_PRINTF(gen_var->selected, "  LTS\n"
                                          "  NOTS\n");
        return true;
    case TOKEN_NIL_COALESCING:
        SBUFFER_PRINTF(gen_var->selected, "  PUSHS nil@nil\n"
                                          "  JUMPIFEGQS %s-%d\n"
                                          "  POPS TF@temp0\n"
                                          "  POPS\n"
                                          "  PUSHS TF@temp0\n"
                                          "  JUMP %s-%d\n"
                                          "LABEL %s-%d\n"
                                          "  POPS\n"
                                          "LABEL %s-%d\n",
                                        gen_var->label, gen_var->counter_n,
                                        gen_var->label, gen_var->counter_n+1,
                                        gen_var->label, gen_var->counter_n,
                                        gen_var->label, gen_var->counter_n+1);
        gen_var->counter_n += 2;
        return true;
    default: // for the compiler to be happy
        return false;
    }
}

bool _generate_logical_expression(GenerationVariables *gen_var, ASTNode *ast,
                                  int t, int f) {
    if (ast->token.type != TOKEN_AND
        && ast->token.type != TOKEN_OR
        && ast->token.type != TOKEN_NOT) {
        if (!_generate_arithmetical_expression(gen_var, ast, ast->data_type))
            return false;
        if (t == 0 || t == 1) {
            SBUFFER_PRINTF(gen_var->selected, "  PUSHS bool@true\n",
                                              "  JUMPIFEQS %s-%d\n",
                                                    gen_var->label->string, t);
            if (f == 0 || f == 1) {
                SBUFFER_PRINTF(gen_var->selected, "  JUMP %s-%d\n",
                                                    gen_var->label->string, f);
            }
        } else if (f == 0 || f == 1) {
            SBUFFER_PRINTF(gen_var->selected, "  PUSHS bool@false\n",
                                              "  JUMPIFEQS %s-%d\n",
                                                gen_var->label->string, f);
        }
        return true;
    }

    if (ast->token.type == TOKEN_AND) {
        if (!_generate_logical_expression(
                gen_var, ast->left, gen_var->counter_n++, f)
            || !_generate_logical_expression(gen_var, ast->right, t, f)) {
            return false;
        }
        return true;
    }

    if (ast->token.type == TOKEN_OR) {
        if (!_generate_logical_expression(
                gen_var, ast->left, t, gen_var->counter_n++)
            || !_generate_logical_expression(gen_var, ast->right, t, f)) {
            return false;
        }
        return true;
    }

    // TOKEN_NOT
    if (!_generate_logical_expression(gen_var, ast->left, f, t)) {
        return false;
    }
    return true;
}

bool generate_expression(GenerationVariables *gen_var, ASTNode *ast) {
    char *init_label;
    if (!clone_string(&init_label, gen_var->label->string)) return false;
    if (!sbuffer_printf(gen_var->label, "&%d", gen_var->expr_n++)) {
        free(init_label);
        return false;
    }
    gen_var->counter_n = 0;

    if (!sbuffer_printf(gen_var->selected, "  CREATEFRAME\n"
                                           "  DEFVAR TF@temp0\n"
                                           "  DEFVAR TF@temp1\n")) {
        free(init_label);
        return false;
    }

    if (ast->token.type != TOKEN_AND
        && ast->token.type != TOKEN_OR
        && ast->token.type != TOKEN_NOT) {
        if (!_generate_arithmetical_expression(gen_var, ast, ast->data_type)) {
            free(init_label);
            return false;
        }
    } else {
        gen_var->counter_n = 2;
        if (!_generate_logical_expression(gen_var, ast, 0, 1)) {
            free(init_label);
            return false;
        }
        if (!sbuffer_printf(gen_var->selected, "LABEL %s-0\n"
                                               "  PUSHS bool@true\n"
                                               "  JUMP %send\n"
                                               "LABEL %s-1\n"
                                               "  PUSHS bool@false\n"
                                               "LABEL %send\n",
                            gen_var->label->string, gen_var->label->string,
                            gen_var->label->string, gen_var->label->string)) {
            free(init_label);
            return false;
        }
    }

    bool res = sbuffer_overwrite_content(
        gen_var->label, "%s", init_label);
    free(init_label);
    return res;
}
