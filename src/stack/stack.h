#include <stdbool.h>
#include <stdlib.h>

/**
 * @brief Stack structure type
 *
 */
typedef struct {
    /// @brief index of the top stack item in items array, is -1 if stack is
    /// empty
    int topIndex;

    /// @brief current length of the items array
    int length;

    /// @brief current allocated size of the items array
    int size;

    /// @brief array of stack items which contain saved pointers
    void** items;
} Stack;

/// @brief initial allocated size of the items array
const int INITIAL_N_ITEMS = 8;

/**
 * @brief procedure to initialize a new stack
 *
 * @return Stack* returns a pointer to the created Stack struct
 */
Stack* stack_init();

/**
 * @brief stack push function, adds an item to the top of the stack, if alloced
 * size for the array is full reallocates memory
 *
 * @param stack pointer to the stack to operate upon
 * @param data  pointer to the data which should be added onto the stack
 * @return true if push was successful
 * @return false if push was unsuccessful (fails due to no memory while
 * allocating)
 */
bool stack_push(Stack* stack, void* data);

/**
 * @brief stack pop function, removes the top item of the stack, doesn't do
 * anything if stack is empty
 *
 * @param stack pointer to the stack which should be popped
 * @return void* returns pointer to the data of the popped element or NULL if
 * stack is empty
 */
void* stack_pop(Stack* stack);

/**
 * @brief function which returns the data pointer of the top item
 *
 * @param stack pointer to the stack
 * @return void* NULL if stack is empty, otherwise pointer to the data of the
 * top item
 */
void* stack_top(Stack* stack);

/**
 * @brief function to check whether the stack is empty
 *
 * @param stack pointer to the stack
 * @return true if stack is empty
 * @return false if stack is not empty
 */
bool is_empty(Stack* stack);

/**
 * @brief Removes all elements of the stack and re-initializes the stack
 *
 * @param stack pointer to the stack
 * @return true if re-initialization was successful
 * @return false if re-initialization was not successful
 */
bool stack_remove_elements(Stack* stack);

/**
 * @brief Removes all elements and frees the stack completely
 * ONLY use if you wish to completely dispose of the stack
 *
 * @param stack pointer to the stack
 */
void stack_free(Stack* stack);