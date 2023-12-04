/**
 * @name Implementation of IFJ23 language compiler
 * @file utils.h
 * @brief Contains definitions of the utility functions.
 *
 * @author Jan Klanica (xklani00)
 */

#ifndef UTILS_H
#define UTILS_H

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stdarg.h>
#include <stdio.h>

/**
 * @brief Allocates memory space for the src string and copies its value to the
 * dest string. Dest string has to be freed afterwards.
 * 
 * @return true If operation was successful.
 *         false If malloc error occured.
*/
bool clone_string(char **dest, char *src);

/**
 * @brief Mallocs new string containing string according to format and args.
 * 
 * @param format Format of the new string.
 * @param args Matching arguments to the format.
 * @return char* New string created.
 * @return NULL On an malloc error.
 */
char *vformatted_string(const char *format, va_list args);

#endif /* UTILS_H */