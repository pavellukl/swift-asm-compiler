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

#define red(str) ANSI_COLOR_RED str ANSI_COLOR_RESET
#define yellow(str) ANSI_COLOR_YELLOW str ANSI_COLOR_RESET

#define printf_stderr(fmt, ...) \
        do { if (DEBUG_TEST) { fprintf(stderr, red("Error: " fmt), ##__VA_ARGS__); } else { fprintf(stderr, "Error: " fmt, ##__VA_ARGS__); } } while(0);

#define printf_stddebug(fmt, ...) \
        do { if (DEBUG_TEST) { fprintf(stddebug, yellow("%s:%d:%s(): " fmt), __FILE__, __LINE__, __func__, ##__VA_ARGS__); } } while(0);

#endif /* DEBUG_H */