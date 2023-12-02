/**
 * @file main.c
 * @brief Contains main body of the console compiler aplication.
 * 
 * @author Jan Klanica (xklani00)
 */

#include <stdio.h>
#include "debug.h"
#include "compiler/compiler.h"

int main() {
    // TODO: remove
    // for debugging purposes
    // FILE* in = fopen(
    //     "./tests/syntax_semantics/test_files/"
    //     "x_example3.swift",
    //     "r");
    // TODO: remove pointer and address it directly
    FILE* in = stdin;

    switch (compile(in, stdout)) {
        case COMP_OK:
            return 0;
        case COMP_LEX_ERR:
            return 1;
        case COMP_STX_ERR:
            return 2;
        case COMP_DEF_ERR:
            return 3;
        case COMP_FNCALL_ERR:
            return 4;
        case COMP_UNDEFVAR_ERR:
            return 5;
        case COMP_FNRET_ERR:
            return 6;
        case COMP_EXPRTYPE_ERR:
            return 7;
        case COMP_UNDEFTYPE_ERR:
            return 8;
        case COMP_OTHER_ERR:
            return 9;
        case COMP_INTER_ERR:
            return 99;
        default:
            return 99;
    }
}