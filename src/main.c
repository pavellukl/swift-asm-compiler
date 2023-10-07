#include "main.h"

// Note that main can't be unit tested -> write everything in separate modules,
// test them separately, mount them together in modules, test them,
// and do only command line service here

int main (int argc, char *argv[]) {
   printf("Hello world!\n");
   fprintf(stdout, "Running %s with %d arguments\n", *argv, argc);

   printf_stderr("This is an error to stderr, it's prefixed with \"Error: \""
                 "and it's red in the debug mode.\n");
   printf_stddebug("This is a yellow debug log to stderr. It is compiled only"
                   "in the debug mode.\n");

   return 0;
}