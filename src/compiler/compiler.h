#ifndef COMPILER_H
#define COMPILER_H

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>

#include "../debug.h"
#include "../parser/parser.h"
#include "../parser/recursive_parser/recursive_parser.h"
#include "../semantic_analyser/semantic_analyser.h"

/// @brief Possible return values of compiler.
typedef enum {
    COMP_OK,
    COMP_LEX_ERR,
    COMP_STX_ERR,
    COMP_DEF_ERR,
    COMP_FNCALL_ERR,
    COMP_UNDEFVAR_ERR,
    COMP_FNRET_ERR,
    COMP_EXPRTYPE_ERR,
    COMP_UNDEFTYPE_ERR,
    COMP_OTHER_ERR,
    COMP_INTER_ERR
} CompilerReturnCode;

/**
 * @brief Compiles the source code in file \p in to file \p out.
 *
 * @param in pointer to the file with code to be compiled.
 * @param out pointer to the output file.
 * @return see documnetation of the task
 */
CompilerReturnCode compile(FILE *in, FILE *out);

CompilerReturnCode _get_return_code(ParserReturnCode rp_retrun_value);

#endif /* COMPILER_H */