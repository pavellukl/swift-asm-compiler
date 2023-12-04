/**
 * @name Implementation of IFJ23 language compiler
 * @file debug.h
 * @brief Contains definitions of debuging macros.
 *
 * @author Jan Klanica (xklani00)
 */

#ifndef DEBUG_H
#define DEBUG_H

#ifdef DEBUG
#define DEBUG_TEST 1
#else
#define DEBUG_TEST 0
#endif

#define ANSI_COLOR_RED "\x1b[31m"
#define ANSI_COLOR_YELLOW "\x1b[0;33m"
#define ANSI_COLOR_RESET "\x1b[0m"

#define stddebug stderr

#define RED(str) ANSI_COLOR_RED str ANSI_COLOR_RESET
#define YELLOW(str) ANSI_COLOR_YELLOW str ANSI_COLOR_RESET

#define PRINTF_STDERR(fmt, ...)                                                \
        do { if (DEBUG_TEST) {                                                 \
                fprintf(stderr, RED("Error: " fmt), ##__VA_ARGS__);            \
        } else {                                                               \
                fprintf(stderr, "Error: " fmt, ##__VA_ARGS__);                 \
        } } while(0);

#define PRINTF_STDDEBUG(fmt, ...)                                              \
        do { if (DEBUG_TEST) {                                                 \
                fprintf(stddebug, YELLOW("%s:%d:%s(): " fmt), __FILE__,        \
                __LINE__, __func__, ##__VA_ARGS__);                            \
        } } while(0);

#endif /* DEBUG_H */