#include "../../src/compiler/compiler.h"

#include <criterion/criterion.h>
#include <criterion/new/assert.h>
#include <dirent.h>
#include <stdio.h>
#include <time.h>
#include <stdbool.h>

#define ANSI_COLOR_RED_ "\x1b[0;31m"
#define ANSI_COLOR_GREEN "\x1b[0;32m"
#define ANSI_COLOR_BLUE "\x1b[0;34m"
#define ANSI_COLOR_RESET "\x1b[0m"
#define ANSI_CURSOR_BACK_7 "\x1b[7D"
#define ANSI_CURSOR_UP "\x1b[1A"

#define PASS "[" ANSI_COLOR_GREEN "PASS" ANSI_COLOR_RESET "]"
#define RUN "[" ANSI_COLOR_BLUE "RUN" ANSI_COLOR_RESET " ]"
#define ERR "[" ANSI_COLOR_RED_ "FAIL" ANSI_COLOR_RESET "]"

Test(syntax_semantics, general) {
    DIR *dir = opendir("./tests/syntax_semantics/test_files");
    if (dir == NULL) {
        cr_fatal("Couldn't open test directory.");
    }

    struct dirent *subdirPtr;

    bool test_failed = false;

    while ((subdirPtr = readdir(dir)) != NULL) {
        if (!strcmp(subdirPtr->d_name, ".") ||
            !strcmp(subdirPtr->d_name, "..")) {
            continue;
        }

        char path[512];
        sprintf(
            path, "./tests/syntax_semantics/test_files/%s", subdirPtr->d_name);

        FILE *in = fopen(path, "r");
        if (in == NULL) {
            cr_fatal("Couldn't open test file %s", subdirPtr->d_name);
        }

        CompilerReturnCode expectedRes;
        fscanf(in, "// %d", (int *)&expectedRes);

        cr_log_info(
            ANSI_CURSOR_BACK_7 RUN " syntax_semantics::%s", subdirPtr->d_name);

        clock_t start = clock();
        CompilerReturnCode res = compile(in, NULL);
        clock_t end = clock();

        if (res != expectedRes) {
            test_failed = true;
            cr_log_error(
                ANSI_CURSOR_BACK_7 ANSI_CURSOR_UP ERR
                " syntax_semantics::%s. Expected: %d but compiler returned %d",
                subdirPtr->d_name, expectedRes, res);
        } else {
            float seconds = (float)(end - start) / CLOCKS_PER_SEC;
            cr_log_info(ANSI_CURSOR_UP ANSI_CURSOR_BACK_7 PASS
                    " syntax_semantics::%s: (%fs)", subdirPtr->d_name, seconds);
        }

        fclose(in);
    }

    cr_assert(test_failed == false);

    closedir(dir);
}
