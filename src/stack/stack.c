#include <stack.h>
#include <stdbool.h>
#include <stdlib.h>

Stack* stack_init() {
    Stack* stack = malloc(sizeof(stack));
    if (stack == NULL) return NULL;

    stack->size = INITIAL_N_ITEMS;
    stack->items = malloc(stack->size * sizeof(void*));
    if (stack->items == NULL) {
        free(stack);
        return NULL;
    }
    stack->length = 0;
    stack->topIndex = -1;

    return stack;
}

bool stack_push(Stack* stack, void* data) {
    if (stack->size == stack->length) {
        void** buffer =
            realloc(stack->items, (stack->size * 2) * sizeof(void*));
        if (buffer == NULL) {
            free(stack->items);
            return false;
        }

        stack->size *= 2;
        stack->items = buffer;
    }

    stack->topIndex++;
    stack->items[stack->topIndex] = data;

    stack->length++;

    return true;
}

void* stack_pop(Stack* stack) {
    if (stack->length == 0) return NULL;

    stack->topIndex--;
    stack->length--;

    return stack->items[stack->topIndex + 1];
}

void* stack_top(Stack* stack) {
    if (stack->length == 0) return NULL;
    return stack->items[stack->topIndex];
}

bool is_empty(Stack* stack) { return stack->length == 0; }

bool stack_remove_elements(Stack* stack) {
    while (stack_pop(stack))
        ;

    stack->topIndex = -1;
    stack->length = 0;

    void** buffer = realloc(stack->items, INITIAL_N_ITEMS * sizeof(void*));
    if (buffer == NULL) {
        free(stack->items);
        return false;
    }

    stack->size = INITIAL_N_ITEMS;
    stack->items = buffer;

    return true;
}

void stack_free(Stack* stack) {
    free(stack->items);
    free(stack);
    stack = NULL;
}
