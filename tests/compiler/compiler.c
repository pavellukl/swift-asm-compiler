#include "../../src/compiler/compiler.h"

#include <criterion/criterion.h>
#include <criterion/new/assert.h>
#include <dirent.h>
#include <stdio.h>

Test(compile, general) {
    DIR *dir = opendir("./tests/compiler/test_files");
    if (dir == NULL) {
        cr_fatal("Couldn't open test directory.");
    }

    struct dirent *subdirPtr;

    while ((subdirPtr = readdir(dir)) != NULL) {
        if (!strcmp(subdirPtr->d_name, ".") ||
            !strcmp(subdirPtr->d_name, "..")) {
            continue;
        }

        char path[512];
        sprintf(path, "./tests/compiler/test_files/%s", subdirPtr->d_name);

        FILE *in = fopen(path, "r");
        if (in == NULL) {
            cr_fatal("Couldn't open test file %s", subdirPtr->d_name);
        }

        CompilerReturnCode expectedRes;
        fscanf(in, "// %d", (int *)&expectedRes);

        cr_log_info("Running %s", subdirPtr->d_name);

        CompilerReturnCode res = compile(in, stdout);

        if (res != expectedRes) {
            cr_log_error(
                "Test failed for file %s. Expected: %d but compiler returned "
                "%d",
                subdirPtr->d_name, expectedRes, res);
        } else {
            cr_log_info("Test successful");
        }

        fclose(in);
    }

    closedir(dir);
}
