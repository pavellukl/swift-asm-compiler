/**
 * @name Implementation of IFJ23 language compiler
 * @file parser.h
 * @brief Header file containing structures for parsing and wrapper functions
 * for get_next_token functions
 *
 * @author Jan Klanica (xklani00)
 * @author Pavel Lukl (xluklp00)
 */

#ifndef PARSER_H
#define PARSER_H

#include "../generation/generation.h"
#include "../scanner/scanner_types.h"
#include "../symtable/symtable.h"

typedef enum {
    OK,
    LEX_ERR,
    STX_ERR,
    DEF_ERR,
    FNCALL_ERR,
    UNDEFVAR_ERR,
    FNRET_ERR,
    EXPRTYPE_ERR,
    UNDEFTYPE_ERR,
    OTHER_ERR,
    INTER_ERR
} ParserReturnCode;

typedef struct {
    LSTElement *current_fnc;
    bool has_function_all_returns;
    unsigned int n_scope_commands;
    bool is_eol_before_required;
} SemanticContext;

typedef struct {
    bool is_first_run;
    ScannerOptions sc_opt;
    TokenData token;
    ListST *symtable;
    ParserReturnCode return_code;
    // for easily passing data between functions in semantic analysis
    SemanticContext sem_ctx;
    GenerationVariables gen_var;
} ParserOptions;

#include "../semantic_analyser/semantic_analyser.h"
#include "../scanner/scanner_func.h"

/**
 * @brief wrapper function for getting the next token, also handles newlines
 * where necessary
 *
 * @param parser_opt parser options struct pointer which contains data that
 * needs to be passed around throughout the compiler
 */
bool _next_token(ParserOptions *parser_opt);
/**
 * @brief wrapper function for getting the first token
 *
 * @param parser_opt parser options struct pointer which contains data that
 * needs to be passed around throughout the compiler
 */
bool _first_token(ParserOptions *parser_opt);

#endif /* PARSER_H */