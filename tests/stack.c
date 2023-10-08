#include "../src/stack/stack.h"

#include <criterion/criterion.h>
#include <stdio.h>

Test(stack, init_and_free) {
    Stack *stack = stack_init();

    cr_assert(stack != NULL);
    cr_assert(stack->length == 0);
    cr_assert(stack->topIndex == -1);
    cr_expect(stack->items != NULL);
    cr_assert(stack->size == INITIAL_N_ITEMS);

    stack_free(stack);
}

Test(stack, push) {
    Stack *stack = stack_init();

    char hello[128];
    for (int i = 0; i < 8; i++) {
        sprintf(hello, "{%d} Hello World!", i);
        stack_push(stack, hello);

        cr_assert(stack->topIndex == (-1 + (i + 1)));
        cr_assert(strcmp(stack->items[stack->topIndex], hello) == 0);
        cr_assert(stack->length = (i + 1));
        cr_assert(stack->size == INITIAL_N_ITEMS);
    }

    sprintf(hello, "{%d} Hello World!", 9);
    stack_push(stack, hello);

    cr_assert(stack->topIndex == 8);
    cr_assert(strcmp(stack->items[stack->topIndex], hello) == 0);
    cr_assert(stack->length = 9);
    cr_assert(stack->size == INITIAL_N_ITEMS * 2);
}
