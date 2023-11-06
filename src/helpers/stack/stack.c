#include "stack.h"

#include <stdbool.h>
#include <stdlib.h>

/*
 * Macro generating implementation of functions operating upon stack.
 *
 * @param T Data type of items stored in the stack.
 * @param TNAME_PASCAL Any name for chosen data type in pascal case.
 * @param TNAME_SNAKE Any name for chosen data type in snake case.
 */
#define STACK_DEF(T, TNAME_PASCAL, TNAME_SNAKE)                               \
                                                                              \
    Stack##TNAME_PASCAL* stack_##TNAME_SNAKE##_init() {                       \
        Stack##TNAME_PASCAL* stack = malloc(sizeof(Stack##TNAME_PASCAL));     \
        if (stack == NULL) return NULL;                                       \
                                                                              \
        stack->size = STACK_INITIAL_N_ITEMS;                                  \
        stack->items = malloc(stack->size * sizeof(T));                       \
        if (stack->items == NULL) {                                           \
            free(stack);                                                      \
            return NULL;                                                      \
        }                                                                     \
                                                                              \
        stack->length = 0;                                                    \
        stack->topIndex = -1;                                                 \
                                                                              \
        return stack;                                                         \
    }                                                                         \
                                                                              \
    bool stack_##TNAME_SNAKE##_push(Stack##TNAME_PASCAL* stack, T data) {     \
        if (stack->size == stack->length) {                                   \
            T* buffer = realloc(stack->items, (stack->size * 2) * sizeof(T)); \
            if (buffer == NULL) {                                             \
                free(stack->items);                                           \
                return false;                                                 \
            }                                                                 \
                                                                              \
            stack->size *= 2;                                                 \
            stack->items = buffer;                                            \
        }                                                                     \
                                                                              \
        stack->topIndex++;                                                    \
        stack->items[stack->topIndex] = data;                                 \
                                                                              \
        stack->length++;                                                      \
                                                                              \
        return true;                                                          \
    }                                                                         \
                                                                              \
    bool stack_##TNAME_SNAKE##_pop(Stack##TNAME_PASCAL* stack, T* data) {     \
        if (stack->length == 0 || data == NULL) return false;                 \
                                                                              \
        *data = stack->items[stack->topIndex];                                \
        stack->topIndex--;                                                    \
        stack->length--;                                                      \
                                                                              \
        if ((stack->size / 2) >= stack->length) {                             \
            T* buffer = realloc(stack->items, (stack->size / 2) * sizeof(T)); \
            if (buffer == NULL) {                                             \
                free(stack->items);                                           \
                return false;                                                 \
            }                                                                 \
                                                                              \
            stack->size /= 2;                                                 \
            stack->items = buffer;                                            \
        }                                                                     \
                                                                              \
        return true;                                                          \
    }                                                                         \
                                                                              \
    bool stack_##TNAME_SNAKE##_top(Stack##TNAME_PASCAL* stack, T* data) {     \
        if (stack->length == 0 || data == NULL) return false;                 \
                                                                              \
        *data = stack->items[stack->topIndex];                                \
        return true;                                                          \
    }                                                                         \
                                                                              \
    bool stack_##TNAME_SNAKE##_is_empty(Stack##TNAME_PASCAL* stack) {         \
        return stack->length == 0;                                            \
    }                                                                         \
                                                                              \
    bool stack_##TNAME_SNAKE##_remove_elements(Stack##TNAME_PASCAL* stack) {  \
        stack->topIndex = -1;                                                 \
        stack->length = 0;                                                    \
                                                                              \
        T* buffer = realloc(stack->items, STACK_INITIAL_N_ITEMS * sizeof(T)); \
        if (buffer == NULL) {                                                 \
            free(stack->items);                                               \
            return false;                                                     \
        }                                                                     \
                                                                              \
        stack->size = STACK_INITIAL_N_ITEMS;                                  \
        stack->items = buffer;                                                \
                                                                              \
        return true;                                                          \
    }                                                                         \
                                                                              \
    void stack_##TNAME_SNAKE##_free(Stack##TNAME_PASCAL* stack) {             \
        free(stack->items);                                                   \
        free(stack);                                                          \
    }

STACK_DEF(TokenData, TokenData, token_data)