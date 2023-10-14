#ifndef COMPILER_H
#define COMPILER_H

#include <stdio.h>
#include <stdbool.h>
#include <assert.h>
#include "../parsing/parsing.h"

/// @brief Possible return values of compiler. TODO: expand values
typedef enum {COMP_OK, COMP_ERROR} CompilerReturnValue;

/// @brief Type for compiler parameters.
typedef struct {
    /// @brief Parameters of syntactic analysis.
    ParsingParams parsing_params;
} CompileParams;

/**
 * @brief Compiles the source code in file \p in to file \p out.
 *
 * @param in pointer to the file with code to be compiled.
 * @param out pointer to the output file.
 * @param params necessary parameters for compilation.
 * @return OK if compilation was successful.
 * @return ERROR if compilation was unsuccessful.
 */
CompilerReturnValue compile(const FILE *in, const FILE *out, 
                            const CompileParams params);

#endif /* COMPILER_H */