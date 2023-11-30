/**
 * @file string_buffer.h
 * @brief Contains definitions of structs and functions for the string buffer.
 * 
 * @author Jan Klanica (xklani00)
 */

#ifndef STRING_BUFFER_H
#define STRING_BUFFER_H

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdbool.h>
#include <string.h>
#include "../utils.h"

#define SBUFFER_INIT_CAPACITY 64

/** Struct representing buffered string. */
typedef struct {
    /** Pointer to the buffered string. */
    char *string;
    /** Size of the buffered string including end of string character. */
    size_t size;
    /** Size of allocated space for buffered string. */
    size_t capacity;
} SBuffer;

SBuffer *sbuffer_init();

void sbuffer_discard(SBuffer *sbuffer);

#define SBUFFER_PRINTF(...)                                                    \
    do { if (!sbuffer_printf(__VA_ARGS__)) return false; } while(0)

bool sbuffer_printf(SBuffer *sbuffer, const char *format, ...);

#define SBUFFER_OVERWRITE_CONTENT(...)                                         \
    do { if (!sbuffer_overwrite_content(__VA_ARGS__)) return false; } while(0)

bool sbuffer_overwrite_content(SBuffer *sbuffer, const char *format, ...);

bool sbuffer_reinit(SBuffer **sbuffer);

#endif /* STRING_BUFFER_H */
