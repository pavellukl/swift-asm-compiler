#ifndef PARAMETERS_H
#define PARAMETERS_H

#include <stdbool.h>

#include "../../symtable/ListST/ListST.h"

#define PARAM_ARR_INC_N_ITEMS 4

/**
 * @brief initialize helper parameter array variable
 *
 * @param params pointer to the parameters struct which's array will be
 * initialized
 */
void init_parameter_array(Parameters *params);

/**
 * @brief add a parameter into parameter array helper
 *
 * @param params pointer to the parameters struct into which the parameter will
 * be added
 * @return true if the operation was successful
 * @return false if the operation was not successful
 */
bool add_to_parameter_array(Parameters *params, Parameter param);

/**
 * @brief invalidates all values in the array, making it work like it's empty
 *
 * @param params pointer to the parameters struct which's array will be
 * invalidated
 */
void invalidate_parameter_array(Parameters *params);

/**
 * @brief unallocates parameter array and parameter strings
 *
 * @param params pointer to the parameters struct which's array will be
 * unallocated
 */
void destroy_parameter_array(Parameters *params);

#endif /* PARAMETERS_H */
