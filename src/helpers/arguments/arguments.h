/**
 * @file arguments.h
 * @brief TODO: file header
 * 
 * @author your name (you@domain.com)
 */

#ifndef ARGUMENTS_H
#define ARGUMENTS_H

#include <stdbool.h>

#include "../../symtable/ListST/ListST.h"

#define ARGS_ARR_INC_N_ITEMS 4

/**
 * @brief initializes arguments array structure
 *
 * @param arguments pointer to the arguments struct which's array will be
 * initialized
 */
void init_argument_array(Arguments *arguments);

/**
 * @brief add an argument into argument array
 *
 * @param arguments pointer to the arguments struct into which the argument will
 * be added
 * @param
 * @return true if the operation was successful
 * @return false if the operation was not successful
 */
bool add_to_argument_array(Arguments *arguments, Argument argument);

/**
 * @brief invalidates all values in the array, making it work like it's empty
 *
 * @param arguments pointer to the arguments struct which's array will be
 * invalidated
 */
void invalidate_argument_array(Arguments *arguments);

/**
 * @brief unallocates argument array and argument strings
 *
 * @param arguments pointer to the arguments struct which's array will be
 * unallocated
 */
void destroy_argument_array(Arguments *arguments);

#endif /* ARGUMENTS_H */
