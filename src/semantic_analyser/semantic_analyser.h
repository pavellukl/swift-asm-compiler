#ifndef SEMANTIC_ANALYSER_H
#define SEMANTIC_ANALYSER_H

#include "../parser/parser.h"

#define PARAM_ARR_INC_N_ITEMS 4

/**
 * @brief analyzes function declaration
 *
 * @param parser_opt parser options pointer to save possible semantic error code
 */
void analyze_function_dec(ParserOptions *parser_opt);

/**
 * @brief initialize helper parameter array variable, beware memory leak
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
bool add_to_parameter_array(Parameters *params, Parameter new_param);

/**
 * @brief invalidates all values in the array, making it work like it's empty
 *
 * @param params pointer to the parameters struct which's array will be
 * invalidated
 */
void invalidate_parameter_array(Parameters *params);

#endif /* SEMANTIC_ANALYSER_H */