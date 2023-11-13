#include "../src/scanner/StackTokenData/StackTokenData.h"

#include <criterion/criterion.h>
#include <stdio.h>

TokenData td_arr[9] = {
    {.line_index = 1, .type = TOKEN_IDENTIF, .value = {"variable_1"}},
    {.line_index = 2, .type = TOKEN_KEYWORD_INT, .value = {"variable_2"}},
    {.line_index = 3, .type = TOKEN_KEYWORD_INT_NIL, .value = {"variable_3"}},
    {.line_index = 4, .type = TOKEN_KEYWORD_DOUBLE, .value = {"variable_4"}},
    {.line_index = 5,
     .type = TOKEN_KEYWORD_DOUBLE_NIL,
     .value = {"variable_5"}},
    {.line_index = 6, .type = TOKEN_KEYWORD_STRING, .value = {"variable_6"}},
    {.line_index = 7,
     .type = TOKEN_KEYWORD_STRING_NIL,
     .value = {"variable_7"}},
    {.line_index = 8, .type = TOKEN_KEYWORD_NIL, .value = {"variable_8"}},
    {.line_index = 9, .type = TOKEN_KEYWORD_LET, .value = {"variable_9"}}};

Test(stack, init_free) {
    StackTokenData *stack = stack_token_data_init();

    cr_assert(stack != NULL);
    cr_assert(stack->length == 0);
    cr_assert(stack->topIndex == -1);
    cr_expect(stack->items != NULL);
    cr_assert(stack->size == STACK_INITIAL_N_ITEMS);

    stack_token_data_free(stack);
}

Test(stack, push_pop) {
    StackTokenData *stack = stack_token_data_init();

    char token_val[128];
    for (int i = 0; i < 8; i++) {
        cr_assert(stack_token_data_push(stack, td_arr[i]));

        sprintf(token_val, "variable_%d", i + 1);

        cr_assert(
            strcmp(stack->items[stack->topIndex].value.string, token_val) == 0);
        cr_assert(stack->items[stack->topIndex].line_index == i + 1);

        cr_assert(stack->topIndex == (-1 + (i + 1)));
        cr_assert(stack->length = (i + 1));
        cr_assert(stack->size == STACK_INITIAL_N_ITEMS);
    }

    cr_assert(stack_token_data_push(stack, td_arr[8]));

    sprintf(token_val, "variable_%d", 9);

    cr_assert(strcmp(stack->items[stack->topIndex].value.string, token_val) ==
              0);
    cr_assert(stack->items[stack->topIndex].line_index == 9);

    cr_assert(stack->topIndex == 8);
    cr_assert(stack->length = 9);
    cr_assert(stack->size == STACK_INITIAL_N_ITEMS * 2);

    TokenData td_tmp;
    cr_assert(stack_token_data_pop(stack, &td_tmp));

    cr_assert(strcmp(td_tmp.value.string, token_val) == 0);
    cr_assert(td_tmp.line_index == 9);

    cr_assert(stack->topIndex == 7);
    cr_assert(stack->length = 8);
    cr_assert(stack->size == STACK_INITIAL_N_ITEMS);

    stack_token_data_free(stack);
}

Test(stack, top) {
    StackTokenData *stack = stack_token_data_init();

    char token_val[128];
    for (int i = 0; i < 8; i++) {
        cr_assert(stack_token_data_push(stack, td_arr[i]));

        TokenData td_tmp;
        cr_assert(stack_token_data_top(stack, &td_tmp));

        sprintf(token_val, "variable_%d", i + 1);

        cr_assert(strcmp(td_tmp.value.string, token_val) == 0);
        cr_assert(td_tmp.line_index == i + 1);
    }

    cr_assert(stack->topIndex == 7);
    cr_assert(stack->length = 8);

    stack_token_data_free(stack);
}

Test(stack, is_empty) {
    StackTokenData *stack = stack_token_data_init();
    TokenData td_tmp = {
        .line_index = 42, .type = TOKEN_ARROW, .value = {"variable_1"}};

    cr_assert(stack_token_data_push(stack, td_tmp));
    cr_assert(stack_token_data_push(stack, td_tmp));
    cr_assert(stack_token_data_push(stack, td_tmp));

    cr_assert(stack_token_data_pop(stack, &td_tmp));
    cr_assert(stack_token_data_pop(stack, &td_tmp));
    cr_assert(stack_token_data_pop(stack, &td_tmp));

    cr_assert(stack_token_data_is_empty(stack));

    stack_token_data_free(stack);
}

Test(stack, remove_elements) {
    StackTokenData *stack = stack_token_data_init();
    TokenData td_tmp = {
        .line_index = 42, .type = TOKEN_ARROW, .value = {"variable_1"}};

    for (int i = 0; i < 16; i++) {
        cr_assert(stack_token_data_push(stack, td_tmp));
    }

    cr_assert(stack_token_data_remove_elements(stack));

    cr_assert(stack->topIndex == -1);
    cr_assert(stack->length == 0);
    cr_assert(stack->size == STACK_INITIAL_N_ITEMS);
    cr_assert(stack->items != NULL);

    stack_token_data_free(stack);
}
