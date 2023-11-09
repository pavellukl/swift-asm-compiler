#ifndef PRECEDENCE_PARSER_H
#define PRECEDENCE_PARSER_H

#include "../scanner/scanner.h"
#include "parser.h"

/**
 * @brief Operation that occurs in precedence parsing.
 * Operations stand for `=`, `<`, `>`, `ERR` in their defined order.
 */
typedef enum { PP_SHIFT_REDUCE, PP_HANDLE_SHIFT, PP_REDUCE, PP_ERROR } PPOperation;

/// @brief List item variant.
typedef enum { PP_TERMINAL, PP_NONTERMINAL, PP_HANDLE, PP_START } PPListItemVariant;

/// @brief Nonterminal which occurs in precedence parsing.
typedef enum { _placeholder_ } PPNonterminal;

/**
 * @brief Represents an item in the precedence list.
 * 
 * Its variant is one of `PP_TERMINAL`, `PP_NONTERMINAL`, `PP_HANDLE`, `PP_START`.
 * Fields are for each variant specified as follows. Others are undefined.
 * ╔═════════════════╦══════════════════════════════════════╗
 * ║     Variant     ║                Fields                ║
 * ╠═════════════════╬══════════════════════════════════════╣
 * ║ PP_TERMINAL     ║ type.token, value.[depends on token] ║
 * ╠═════════════════╬══════════════════════════════════════╣
 * ║ PP_NONTERMINAL  ║ type.nonterminal                     ║
 * ╠═════════════════╬══════════════════════════════════════╣
 * ║ PP_HANDLE       ║                                      ║
 * ╠═════════════════╬══════════════════════════════════════╣
 * ║ PP_START        ║                                      ║
 * ╚═════════════════╩══════════════════════════════════════╝
 */
typedef struct {
  PPListItemVariant variant;
  union {
    TokenType token;
    PPNonterminal nonterminal;
  } type;
  // value
  union {
      char* string;
      int int_;
      double float_;
  } value;
} PPListItem;

// /**
//  * @brief Precedence table type.
//  *
//  * The table is of size m*n, where m represents terminal and n represents
//  * token.
//  */
// typedef PrecedentOperation **PrecedentTable;

bool parse_check_optimize_generate_expression(ParserOptions *parser_opt);

#endif /* PRECEDENCE_PARSER_H */