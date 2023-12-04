/**
 * @name Implementation of IFJ23 language compiler
 * @file string_buffer.c
 * @brief Contains the implementation of the string buffer.
 *
 * @author Jan Klanica (xklani00)
 */

#include "string_buffer.h"

SBuffer *sbuffer_init() {
    SBuffer *sbuffer = malloc(sizeof (*sbuffer));
    if (sbuffer == NULL) {
        return NULL;
    }

    sbuffer->string = malloc(sizeof (*sbuffer->string) * SBUFFER_INIT_CAPACITY);
    if (sbuffer->string == NULL) {
        free(sbuffer);
        return NULL;
    }

    sbuffer->string[0] = '\0';
    sbuffer->capacity = SBUFFER_INIT_CAPACITY;
    sbuffer->size = 1;

    return sbuffer;
}

void sbuffer_discard(SBuffer *sbuffer) {
    free(sbuffer->string);
    free(sbuffer);
}

bool sbuffer_printf(SBuffer *sbuffer, const char *format, ...) {
    va_list args;
    va_start(args, format);
    char *message = vformatted_string(format, args);
    if (message == NULL) return false;
    va_end(args);

    int append_size = strlen(message);

    if (sbuffer->capacity < sbuffer->size + append_size) {
        while (sbuffer->capacity < sbuffer->size + append_size)
        {
            sbuffer->capacity *= 2;
        }
        
        sbuffer->string = realloc(sbuffer->string,
                                  sizeof(*sbuffer->string) * sbuffer->capacity);
        if (sbuffer->string == NULL) {
            sbuffer->capacity = 0;
            sbuffer->size = 0;
            free(message);
            return false;
        }
    }

    strcat(sbuffer->string, message);
    sbuffer->size += append_size;

    free(message);
    return true;
}

bool sbuffer_overwrite_content(SBuffer *sbuffer, const char *format, ...) {
    sbuffer->string[0] = '\0';
    sbuffer->size = 1;

    va_list args;
    va_start(args, format);
    char *message = vformatted_string(format, args);
    if (message == NULL) return false;
    va_end(args);

    bool res = sbuffer_printf(sbuffer, "%s", message);

    free(message);
    return res;
}

bool sbuffer_reinit(SBuffer **sbuffer) {
    sbuffer_discard(*sbuffer);
    *sbuffer = sbuffer_init();
    if (*sbuffer == NULL) {
        return false;
    }
    return true;
}
