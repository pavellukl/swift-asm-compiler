#ifndef UTILS_H
#define UTILS_H

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

/**
 * @brief Allocates memory space for the src string and copies its value to the
 * dest string. Dest string has to be freed afterwards.
 * 
 * @return true If operation was successful.
 *         false If malloc error occured.
*/
bool clone_string(char **dest, char *src);

#endif /* UTILS_H */