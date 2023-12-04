/**
 * @name Implementation of IFJ23 language compiler
 * @file compiler.c
 * @brief Contains definitions of structs and functions for the compiler.
 *
 * @author Jan Klanica (xklani00)
 * @author Pavel Lukl (xklukl00)
 */

#ifndef COMPILER_H
#define COMPILER_H

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>

#include "../debug.h"
#include "../parser/parser.h"
#include "../parser/recursive_parser/recursive_parser.h"
#include "../semantic_analyser/semantic_analyser.h"

/// @brief Possible return values of the compiler.
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
    COMP_INTER_ERR = 99
} CompilerReturnCode;

/**
 * @brief Compiles the source code in file \p in to file \p out.
 *
 * @param in pointer to the file with code to be compiled.
 * @param out pointer to the output file.
 * @return see documnetation of the task
 */
CompilerReturnCode compile(FILE *in, FILE *out);

/**
 * @brief Converts parser return code to the compiler return code.
 * 
 * @param rp_retrun_value Parser return code to be converted.
 * @return CompilerReturnCode
 */
CompilerReturnCode _get_return_code(ParserReturnCode rp_retrun_value);

#endif /* COMPILER_H */