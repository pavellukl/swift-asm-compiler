#ifndef GENERATION_H
#define GENERATION_H

#include <stdio.h>
#include <stdbool.h>
#include "../helpers/string_buffer/string_buffer.h"

typedef struct {
    SBuffer *main;
    SBuffer *functions;
    SBuffer *scope;
    int scope_n;
    int if_n;
    int while_n;
    int lexpr_n;
} GenerationVariables;


bool generation_init(GenerationVariables gen_opt);
void generation_free(const GenerationVariables gen_opt);
void print_generation_to_file(GenerationVariables gen_opt, FILE *out);

void generate_inbuilt_functions(GenerationVariables gen_var);

#endif /* GENERATION_H */