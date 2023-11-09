#include "../src/compiler/compiler.h"

#include <criterion/criterion.h>
#include <dirent.h>
#include <stdio.h>

Test(compile, general) {
    DIR *dir = opendir("./test_files");
    if (dir == NULL) {
        cr_fatal("TEST FAILED: Couldn't open test dir");
    }

    struct dirent *subdirPtr;

    while ((subdirPtr = readdir(dir)) != NULL) {
        printf("%s", subdirPtr->d_name);
    }

    closedir(dir);
}
