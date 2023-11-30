/**
 * @file utils.c
 * @brief Contains the implementation of the utility functions.
 *
 * @author Jan Klanica (xklani00)
 */

#include "utils.h"

bool clone_string(char **dest, char *src) {
    *dest = malloc(sizeof(char) * (strlen(src) + 1));
    if (*dest == NULL) {
        return false;
    }
    strcpy(*dest, src);
    return true;
}

char *vformatted_string(const char *format, va_list args) {
    va_list args_copy;
    va_copy(args_copy, args);

    size_t append_size = vsnprintf(NULL, 0, format, args);
    char *append = malloc(append_size + 1);
    if (append == NULL) {
        return NULL;
    }

    vsprintf(append, format, args_copy);

    return append;
}