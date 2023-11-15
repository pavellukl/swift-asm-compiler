#ifndef SEMANTIC_ANALYSER_H
#define SEMANTIC_ANALYSER_H

#include "../parser/parser.h"

/**
 * @brief analyzes function declaration
 * 
 * @param parser_opt parser options pointer to save possible semantic error code
 */
void analyze_function_dec(ParserOptions *parser_opt);

#endif /* SEMANTIC_ANALYSER_H */