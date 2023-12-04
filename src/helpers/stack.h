/**
 * @name Implementation of IFJ23 language compiler
 * @file stack.h
 * @brief Contains definitions of structs, functions and macros for the ADT
 * stack.
 *
 * @author Pavel Lukl (xluklp00)
 */

#ifndef STACK_H
#define STACK_H

#include <stdbool.h>
#include <stdlib.h>

/// @brief initial allocated size of the items array
#define STACK_INITIAL_N_ITEMS 8

/*
 * Macro generating declarations of functions operating upon stack.
 *
 * @param T Data type of items stored in the stack.
 * @param TNAME_PASCAL Any name for chosen data type in pascal case.
 * @param TNAME_SNAKE Any name for chosen data type in snake case.
 */

#define STACK_DEC(T, TNAME_PASCAL, TNAME_SNAKE)                              \
                                                                             \
    /**                                                                      \
     * @brief Stack structure type                                           \
     *                                                                       \
     */                                                                      \
    typedef struct {                                                         \
        /**                                                                  \
        @brief index of the top stack item in items array, is -1 if          \
        stack is empty                                                       \
        */                                                                   \
        int topIndex;                                                        \
                                                                             \
        /** @brief current length of the items array*/                       \
        int length;                                                          \
                                                                             \
        /** @brief current allocated size of the items array */              \
        int size;                                                            \
                                                                             \
        /** @brief array of stack items which contain saved pointers */      \
        T* items;                                                            \
    } Stack##TNAME_PASCAL;                                                   \
                                                                             \
    /**                                                                      \
     * @brief procedure to initialize a new stack                            \
     *                                                                       \
     * @return Stack* returns a pointer to the created Stack struct          \
     */                                                                      \
    Stack##TNAME_PASCAL* stack_##TNAME_SNAKE##_init();                       \
                                                                             \
    /**                                                                      \
     * @brief stack push function, adds an item to the top of the stack, if  \
     * alloced size for the array is full reallocates memory                 \
     *                                                                       \
     * @param stack pointer to the stack to operate upon                     \
     * @param data  data which should be added onto the stack                \
     * @return true if push was successful                                   \
     * @return false if push was unsuccessful (fails due to no memory while  \
     * allocating)                                                           \
     */                                                                      \
    bool stack_##TNAME_SNAKE##_push(Stack##TNAME_PASCAL* stack, T data);     \
                                                                             \
    /**                                                                      \
     * @brief stack pop function, removes the top item of the stack, doesn't \
     * do anything if stack is empty, can also downscale memory              \
     *                                                                       \
     * @param data pointer to where data should be saved                     \
     * @param stack pointer to the stack which should be popped              \
     * @return data of the popped element or NULL if stack is empty          \
     */                                                                      \
    bool stack_##TNAME_SNAKE##_pop(Stack##TNAME_PASCAL* stack, T* data);     \
                                                                             \
    /**                                                                      \
     * @brief function which returns the data pointer of the top item        \
     *                                                                       \
     * @param data pointer to where data should be saved                     \
     * @param stack pointer to the stack                                     \
     * @return false if stack is empty, otherwise true                       \
     */                                                                      \
    bool stack_##TNAME_SNAKE##_top(Stack##TNAME_PASCAL* stack, T* data);     \
                                                                             \
    /**                                                                      \
     * @brief function to check whether the stack is empty                   \
     *                                                                       \
     * @param stack pointer to the stack                                     \
     * @return true if stack is empty                                        \
     * @return false if stack is not empty                                   \
     */                                                                      \
    bool stack_##TNAME_SNAKE##_is_empty(Stack##TNAME_PASCAL* stack);         \
                                                                             \
    /**                                                                      \
     * @brief Removes all elements of the stack and re-initializes the stack \
     *                                                                       \
     * @param stack pointer to the stack                                     \
     * @return true if re-initialization was successful                      \
     * @return false if re-initialization was not successful                 \
     */                                                                      \
    bool stack_##TNAME_SNAKE##_remove_elements(Stack##TNAME_PASCAL* stack);  \
                                                                             \
    /**                                                                      \
     * @brief Removes all elements and frees the stack completely            \
     * ONLY use if you wish to completely dispose of the stack               \
     *                                                                       \
     * @param stack pointer to the stack                                     \
     */                                                                      \
    void stack_##TNAME_SNAKE##_free(Stack##TNAME_PASCAL* stack);


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

#endif