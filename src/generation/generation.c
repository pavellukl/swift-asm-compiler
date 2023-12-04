/**
 * @file generation.c
 * @brief Contains the implementation of the generation.
 *
 * @author Jan Klanica (xklani00)
 * @author Denis Milistenfer (xmilis00)
 * @author Pavel Lukl (xluklp00)
 */

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
        fprintf(out, "  exit int@0\n\n");
        fprintf(out, "%s", gen_opt.functions->string);
    }
}

bool generate_function_beginning(GenerationVariables gen_var, LSTElement *fn) {
    if (fn->variant != FUNCTION) return false;
    if (fn->value.parameters.infinite) return false;

    SBUFFER_PRINTF(gen_var.functions, "\nLABEL _%s\n"
                                      "  CREATEFRAME\n", fn->identifier);

    for (int i = fn->value.parameters.size - 1; i >= 0; i--) {
        SBUFFER_PRINTF(gen_var.functions,
                       "  DEFVAR TF@%s1\n"
                       "  POPS TF@%s1\n",
                       fn->value.parameters.parameters_arr[i].identifier,
                       fn->value.parameters.parameters_arr[i].identifier);
    }

    SBUFFER_PRINTF(gen_var.functions, "  PUSHFRAME\n");

    return true;
}

bool generate_function_end(SBuffer *sbuffer) {
    SBUFFER_PRINTF(sbuffer, "  POPFRAME\n"
                            "  RETURN\n");
    return true;
}

bool generate_inbuilt_functions(GenerationVariables gen_var, ListST *symtable) {
    SBuffer *init_buffer = gen_var.selected;

    gen_var.selected = gen_var.functions;
    int scope;
    LSTElement *el;

    el = st_search_element(symtable, "readString", &scope);
    if (el == NULL || !generate_function_beginning(gen_var, el)) return false;
    SBUFFER_PRINTF(gen_var.selected,
                   "  DEFVAR LF@result\n"
                   "  READ LF@result string\n"
                   "  PUSHS LF@result\n");
    if (!generate_function_end(gen_var.functions)) return false;

    el = st_search_element(symtable, "readInt", &scope);
    if (el == NULL || !generate_function_beginning(gen_var, el)) return false;
    SBUFFER_PRINTF(gen_var.selected,
                   "  DEFVAR LF@result\n"
                   "  READ LF@result int\n"
                   "  PUSHS LF@result\n");
    if (!generate_function_end(gen_var.functions)) return false;

    el = st_search_element(symtable, "readDouble", &scope);
    if (el == NULL || !generate_function_beginning(gen_var, el)) return false;
    SBUFFER_PRINTF(gen_var.selected,
                   "  DEFVAR LF@result\n"
                   "  READ LF@result float\n"
                   "  PUSHS LF@result\n");
    if (!generate_function_end(gen_var.functions)) return false;

    el = st_search_element(symtable, "Int2Double", &scope);
    if (el == NULL || !generate_function_beginning(gen_var, el)) return false;
    SBUFFER_PRINTF(gen_var.selected,
                   "  INT2FLOAT LF@term1 LF@term1\n"
                   "  PUSHS LF@term1\n");
    if (!generate_function_end(gen_var.functions)) return false;

    el = st_search_element(symtable, "Double2Int", &scope);
    if (el == NULL || !generate_function_beginning(gen_var, el)) return false;
    SBUFFER_PRINTF(gen_var.selected,
                   "  FLOAT2INT LF@term1 LF@term1\n"
                   "  PUSHS LF@term1\n");
    if (!generate_function_end(gen_var.functions)) return false;

    el = st_search_element(symtable, "length", &scope);
    if (el == NULL || !generate_function_beginning(gen_var, el)) return false;
    SBUFFER_PRINTF(gen_var.selected,
                   "  STRLEN LF@s1 LF@s1\n"
                   "  PUSHS LF@s1\n");
    if (!generate_function_end(gen_var.functions)) return false;

    el = st_search_element(symtable, "substring", &scope);
    if (el == NULL || !generate_function_beginning(gen_var, el)) return false;

    // conditions returning nil
    SBUFFER_PRINTF(gen_var.selected,
                   "  DEFVAR LF@cond\n"
                   "  DEFVAR LF@length\n"
                   "  LT LF@cond LF@i1 int@0\n"  // i < 0
                   "  JUMPIFEQ _substring_return_nil LF@cond bool@true\n"
                   "  LT LF@cond LF@j1 int@0\n"  // j < 0
                   "  JUMPIFEQ _substring_return_nil LF@cond bool@true\n"
                   "  GT LF@cond LF@i1 LF@j1\n"  // i > j
                   "  JUMPIFEQ _substring_return_nil LF@cond bool@true\n"
                   "  STRLEN LF@length LF@s1\n"
                   "  DEFVAR LF@cond2\n"
                   "  GT LF@cond LF@i1 LF@length\n"  // i >= length(s)
                   "  EQ LF@cond2 LF@i1 LF@length\n"
                   "  OR LF@cond LF@cond LF@cond2\n"
                   "  JUMPIFEQ _substring_return_nil LF@cond bool@true\n"
                   "  GT LF@cond LF@j1 LF@length\n"  // j > length(s)
                   "  JUMPIFEQ _substring_return_nil LF@cond bool@true\n");
    // getting substring
    SBUFFER_PRINTF(gen_var.selected,
                   "  DEFVAR LF@substring\n"
                   "  MOVE LF@substring string@\n"
                   "  DEFVAR LF@string_index\n"
                   "  MOVE LF@string_index LF@i1\n"
                   "  DEFVAR LF@char\n"
                   "LABEL _substring%%0\n"
                   "  JUMPIFEQ _substring%%0end LF@string_index LF@j1\n"
                   "  GETCHAR LF@char LF@s1 LF@string_index\n"
                   "  CONCAT LF@substring LF@substring LF@char\n"
                   "  ADD LF@string_index LF@string_index int@1\n"
                   "  JUMP _substring%%0\n"
                   "LABEL _substring%%0end\n");
    // return
    SBUFFER_PRINTF(gen_var.selected,
                   "  PUSHS LF@substring\n");
    if (!generate_function_end(gen_var.functions)) return false;
    // nil return
    SBUFFER_PRINTF(gen_var.selected,
                   "LABEL _substring_return_nil\n"
                   "  PUSHS nil@nil\n");
    if (!generate_function_end(gen_var.functions)) return false;

    el = st_search_element(symtable, "ord", &scope);
    if (el == NULL || !generate_function_beginning(gen_var, el)) return false;
    SBUFFER_PRINTF(gen_var.selected,
                   "  PUSHS LF@c1\n"
                   "  CALL _length\n"
                   "  PUSHS int@0\n"
                   "  JUMPIFEQS _ord?0t\n"
                   "  STRI2INT LF@c1 LF@c1 int@0\n"
                   "  PUSHS LF@c1\n"
                   "  JUMP _ord?0end\n"
                   "LABEL _ord?0t\n"
                   "  PUSHS int@0\n"
                   "LABEL _ord?0end\n");
    if (!generate_function_end(gen_var.functions)) return false;

    el = st_search_element(symtable, "chr", &scope);
    if (el == NULL || !generate_function_beginning(gen_var, el)) return false;
    SBUFFER_PRINTF(gen_var.selected,
                   "  INT2CHAR LF@i1 LF@i1\n"
                   "  PUSHS LF@i1\n");
    if (!generate_function_end(gen_var.functions)) return false;

    el = st_search_element(symtable, "write", &scope);
    if (el == NULL) return false;
    SBUFFER_PRINTF(gen_var.selected,
                   "\nLABEL _write\n"
                   "  CREATEFRAME\n"
                   "  PUSHFRAME\n");
    // pop the parameter count
    SBUFFER_PRINTF(gen_var.selected,
                   "  DEFVAR LF@param_count\n"
                   "  POPS LF@param_count\n");
    // printing parameters
    SBUFFER_PRINTF(gen_var.selected,
                   "  DEFVAR LF@counter\n"
                   "  MOVE LF@counter int@0\n"
                   "  DEFVAR LF@temp_var\n"
                   "LABEL _write%%0\n"
                   "  JUMPIFEQ _write%%0end LF@counter LF@param_count\n"
                   "  POPS LF@temp_var\n"
                   "  WRITE LF@temp_var\n"
                   "  ADD LF@counter LF@counter int@1\n"
                   "  JUMP _write%%0\n"
                   "LABEL _write%%0end\n");
    if (!generate_function_end(gen_var.functions)) return false;

    gen_var.selected = init_buffer;
    return true;
}

bool generate_assignment(GenerationVariables gen_var, ListST *symtable,
                         char *assign_to_identif) {
    SBUFFER_PRINTF(gen_var.scope, "  POPS ");
    if (!generate_variable(gen_var.scope, symtable, assign_to_identif)) {
        return false;
    }
    SBUFFER_PRINTF(gen_var.scope, "\n");
    return true;
}

bool generate_fnc_call(GenerationVariables gen_var, char *identifier) {
    SBUFFER_PRINTF(gen_var.scope, "  CALL _%s\n", identifier);
    return true;
}


bool generate_variable_definition(GenerationVariables *gen_var,
                                  ListST *symtable, char *identifier,
                                  Type expected_var_type,
                                  ASTNode *provided_expression_node,
                                  bool is_function) {
    // variable declaration
    SBUFFER_PRINTF(gen_var->selected, "  DEFVAR ");
    if (!generate_variable(gen_var->selected, symtable, identifier)) {
        return false;
    }
    SBUFFER_PRINTF(gen_var->selected, "\n");

    // generate assignment rvalue calculation
    if (is_function) {
        // it is a function call return value
        // function call is already generated

    } else if (provided_expression_node->data_type != T_VOID) {
        // it is an expression
        generate_expression(gen_var, provided_expression_node, symtable);

    } else if (expected_var_type == T_INT_NIL ||
               expected_var_type == T_FLOAT_NIL ||
               expected_var_type == T_STRING_NIL ||
               expected_var_type == T_BOOL_NIL) {
        // it is none but the variable is one of the nillable types
        // -> automatically initialized with nil
        SBUFFER_PRINTF(gen_var->scope, "  PUSHS nil@nil\n");

    } else {
        // it is uninitialized
        return true;
    } 

    // generate assignment
    if (!generate_assignment(*gen_var, symtable, identifier)) return false;

    return true;
}

bool generate_argument(GenerationVariables gen_var, ListST *symtable,
                       Argument arg, Type expected_type) {
    SBUFFER_PRINTF(gen_var.scope, "  PUSHS ");

    if (arg.token_type == TOKEN_IDENTIF) {
        if (!generate_variable(gen_var.scope, symtable, arg.identifier)) {
            return false;
        }
    } else {
        TokenData token = {.type = arg.token_type, .value = arg.value};
        if (!generate_literal(gen_var, token, expected_type)) {
            return false;
        }
    }

    SBUFFER_PRINTF(gen_var.scope, "\n");
    return true;
}

bool generate_variable(SBuffer *sbuffer, ListST *symtable,
                        char *identifier) {
    int scope_id;
    LSTElement *el = st_search_element(symtable, identifier, &scope_id);
    if (el == NULL) return false;

    if (scope_id == 0) {
        SBUFFER_PRINTF(sbuffer, "GF@%s", identifier);
    } else {
        SBUFFER_PRINTF(sbuffer, "LF@%s%d", identifier, scope_id);
    }

    return true;
}

bool generate_string_literal(GenerationVariables gen_var, char *str) {
    for (int i = 0; str[i] != '\0'; i++) {
        if ((str[i] >= 0 && str[i] <= 32) || str[i] == 35 || str[i] == 92) {
            SBUFFER_PRINTF(gen_var.scope, "\\0");
            if (str[i] < 10) {
                SBUFFER_PRINTF(gen_var.scope, "0");
            }
            SBUFFER_PRINTF(gen_var.scope, "%d", str[i]);
        } else {
            SBUFFER_PRINTF(gen_var.scope, "%c", str[i]);
        }
    }
    return true;
}

bool generate_literal(GenerationVariables gen_var, TokenData token,
                      Type expected_type) {
    switch (token.type)
    {
    case TOKEN_INT:
        if (expected_type == T_FLOAT) {
            SBUFFER_PRINTF(gen_var.scope, "float@%a",
                           (double)token.value.int_value);
        } else {
            SBUFFER_PRINTF(gen_var.scope, "int@%d", token.value.int_value);
        }
        return true;
    case TOKEN_FLOAT:
        SBUFFER_PRINTF(gen_var.scope, "float@%a", token.value.float_value);
        return true;
    case TOKEN_STRING:
        SBUFFER_PRINTF(gen_var.scope, "string@");
        if (!generate_string_literal(gen_var, token.value.string))
            return false;
        return true;
    case TOKEN_BOOL:
        if (token.value.boolean == true) {
            SBUFFER_PRINTF(gen_var.scope, "bool@true");
        } else {
            SBUFFER_PRINTF(gen_var.scope, "bool@false");
        }
        return true;
    case TOKEN_KEYWORD_NIL:
        SBUFFER_PRINTF(gen_var.scope, "nil@nil");
        return true;
    default:
        return false;
    }
}

bool _generate_simply_expression(GenerationVariables *gen_var, ASTNode *ast,
                                 ListST *symtable, Type expr_type) {
    if (ast->left == NULL && ast->right == NULL) {
        SBUFFER_PRINTF(gen_var->scope, "  PUSHS ");

        // it can be a variable or a literal
        if (ast->token.type == TOKEN_IDENTIF) {
            // this should never fail - variable existence was tested before
            if (!generate_variable(gen_var->scope, symtable,
                                   ast->token.value.string)) {
                return false;
            }

            // type conversion
            // ast->data_type has identifiers data type without nilable part
            // (as opposed to symtable)
            if (expr_type == T_FLOAT && ast->data_type == T_INT) {
                SBUFFER_PRINTF(gen_var->scope, "\n"
                                               "  INT2FLOATS");
            }
        } else {
            if (!generate_literal(*gen_var, ast->token, expr_type))
                return false;
        }

        SBUFFER_PRINTF(gen_var->scope, "\n");
        return true;
    }

    if (expr_type == T_BOOL &&
       (ast->left->data_type == T_FLOAT || ast->right->data_type == T_FLOAT)) {
        expr_type = T_FLOAT;
    }

    if (!_generate_simply_expression(gen_var, ast->left, symtable, expr_type))
        return false;
    if (!_generate_simply_expression(gen_var, ast->right, symtable, expr_type))
        return false;

    switch (ast->token.type)
    {
    case TOKEN_ADD:
        if (ast->data_type == T_STRING) {
            SBUFFER_PRINTF(gen_var->scope,
                                        "  POPS TF@temp1\n"
                                        "  POPS TF@temp0\n"
                                        "  CONCAT TF@temp0 TF@temp0 TF@temp1\n"
                                        "  PUSHS TF@temp0\n");
        } else {
            SBUFFER_PRINTF(gen_var->scope, "  ADDS\n");
        }
        return true;
    case TOKEN_SUB:
        SBUFFER_PRINTF(gen_var->scope, "  SUBS\n");
        return true;
    case TOKEN_MUL:
        SBUFFER_PRINTF(gen_var->scope, "  MULS\n");
        return true;
    case TOKEN_DIV:
        if (ast->left->data_type == T_INT) {
            SBUFFER_PRINTF(gen_var->scope, "  IDIVS\n");
        } else {
            SBUFFER_PRINTF(gen_var->scope, "  DIVS\n");
        }
        return true;
    case TOKEN_EQUAL:
        SBUFFER_PRINTF(gen_var->scope, "  EQS\n");
        return true;
    case TOKEN_NOT_EQUAL:
        SBUFFER_PRINTF(gen_var->scope, "  EQS\n"
                                       "  NOTS\n");
        return true;
    case TOKEN_LESSER:
        SBUFFER_PRINTF(gen_var->scope, "  LTS\n");
        return true;
    case TOKEN_LESSER_EQUAL:
        SBUFFER_PRINTF(gen_var->scope, "  GTS\n"
                                       "  NOTS\n");
        return true;
    case TOKEN_GREATER:
        SBUFFER_PRINTF(gen_var->scope, "  GTS\n");
        return true;
    case TOKEN_GREATER_EQUAL:
        SBUFFER_PRINTF(gen_var->scope, "  LTS\n"
                                       "  NOTS\n");
        return true;
    case TOKEN_NIL_COALESCING:
        SBUFFER_PRINTF(gen_var->scope, "  POPS TF@temp1\n"
                                       "  POPS TF@temp0\n"
                                       "  JUMPIFEQ %s-%d TF@temp0 nil@nil\n"
                                       "  PUSHS TF@temp0\n"
                                       "  JUMP %s-%d\n"
                                       "LABEL %s-%d\n"
                                       "  PUSHS TF@temp1\n"
                                       "LABEL %s-%d\n",
                                gen_var->label->string, gen_var->counter_n,
                                gen_var->label->string, gen_var->counter_n+1,
                                gen_var->label->string, gen_var->counter_n,
                                gen_var->label->string, gen_var->counter_n+1);
        gen_var->counter_n += 2;
        return true;
    default:
        return false;
    }
}

bool _generate_short_circuit_eval(GenerationVariables *gen_var, ASTNode *ast,
                                  ListST *symtable, int t, int f) {
    if (ast->token.type != TOKEN_AND
        && ast->token.type != TOKEN_OR
        && ast->token.type != TOKEN_NOT) {
        if (!_generate_simply_expression(gen_var, ast, symtable,
                                         ast->data_type)) {
            return false;
        }
        SBUFFER_PRINTF(gen_var->scope, "  PUSHS bool@true\n"
                                       "  JUMPIFEQS %s-%d\n"
                                       "  JUMP %s-%d\n",
                                        gen_var->label->string, t,
                                        gen_var->label->string, f);
        return true;
    }

    if (ast->token.type == TOKEN_AND) {
        int new_label_id = gen_var->counter_n++;
        if (!_generate_short_circuit_eval(gen_var, ast->left, symtable,
                                          new_label_id, f)) {
            return false;
        }
        SBUFFER_PRINTF(gen_var->scope, "LABEL %s-%d\n",
                                        gen_var->label->string, new_label_id);
        if (!_generate_short_circuit_eval(gen_var, ast->right, symtable, t, f)){
            return false;
        }
        return true;
    }

    if (ast->token.type == TOKEN_OR) {
        int new_label_id = gen_var->counter_n++;
        if (!_generate_short_circuit_eval(gen_var, ast->left, symtable, t,
                                          new_label_id)) {
            return false;
        }
        SBUFFER_PRINTF(gen_var->scope, "LABEL %s-%d\n",
                                        gen_var->label->string, new_label_id);
        if (!_generate_short_circuit_eval(gen_var, ast->right, symtable, t, f)){
            return false;
        }
        return true;
    }

    // TOKEN_NOT
    if (!_generate_short_circuit_eval(gen_var, ast->left, symtable, f, t)) {
        return false;
    }
    return true;
}

ASTNode* ast_init() {
    ASTNode *node = malloc(sizeof(ASTNode));
    if (node == NULL) {
        return NULL;
    }
    node->data_type = T_VOID;
    node->token.type = TOKEN_KEYWORD_INT;
    node->left = NULL;
    node->right = NULL;
    return node;
}

bool generate_expression(GenerationVariables *gen_var, ASTNode *ast,
                         ListST *symtable) {
    if (ast == NULL || ast->data_type == T_VOID) return true;

    gen_var->expr_n++;

    char *init_label;
    if (!clone_string(&init_label, gen_var->label->string)) return false;
    if (!sbuffer_printf(gen_var->label, "&%d", gen_var->expr_n++)) {
        free(init_label);
        return false;
    }
    gen_var->counter_n = 0;

    if (!sbuffer_printf(gen_var->scope, "  CREATEFRAME\n"
                                        "  DEFVAR TF@temp0\n"
                                        "  DEFVAR TF@temp1\n")) {
        free(init_label);
        return false;
    }

    if (ast->token.type != TOKEN_AND
        && ast->token.type != TOKEN_OR
        && ast->token.type != TOKEN_NOT) {
        if (!_generate_simply_expression(
                gen_var, ast, symtable, ast->data_type)) {
            free(init_label);
            return false;
        }
    } else {
        gen_var->counter_n = 2;
        if (!_generate_short_circuit_eval(gen_var, ast, symtable, 0, 1)) {
            free(init_label);
            return false;
        }
        if (!sbuffer_printf(gen_var->scope, "LABEL %s-0\n"
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

    bool res = sbuffer_overwrite_content(gen_var->label, "%s", init_label);
    free(init_label);
    return res;
}
