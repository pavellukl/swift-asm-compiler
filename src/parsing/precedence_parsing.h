#ifndef PRECEDENCE_PARSING_H
#define PRECEDENCE_PARSING_H

#include "../lexical_analysis/lexical_analysis.h"

/**
 * @brief Operation that occurs in precedence parsing.
 * Operations stand for `=`, `<`, `>`, `err` in their defined order.
 */
typedef enum { PPR_SHIFT, PPR_SHIFT_HANDLE, PPR_REDUCE, PPR_ERROR } PPROperation;

/// @brief List item variant.
typedef enum { PPR_TERMINAL, PPR_NONTERMINAL, PPR_HANDLE, PPR_START } PPRListItemVariant;

/// @brief Nonterminal which occurs in precedence parsing.
typedef enum { _placeholder_ } PPRNonterminal;

/**
 * @brief Represents an item in the precedence list.
 * 
 * Its variant is one of `PPR_TERMINAL`, `PPR_NONTERMINAL`, `PPR_HANDLE`, `PPR_START`.
 * Fields are for each variant specified as follows. Others are undefined.
 * ╔═════════════════╦══════════════════════════════════════╗
 * ║     Variant     ║                Fields                ║
 * ╠═════════════════╬══════════════════════════════════════╣
 * ║ PPR_TERMINAL    ║ type.token, value.[depends on token] ║
 * ╠═════════════════╬══════════════════════════════════════╣
 * ║ PPR_NONTERMINAL ║ type.nonterminal                     ║
 * ╠═════════════════╬══════════════════════════════════════╣
 * ║ PPR_HANDLE      ║                                      ║
 * ╠═════════════════╬══════════════════════════════════════╣
 * ║ PPR_START       ║                                      ║
 * ╚═════════════════╩══════════════════════════════════════╝
 */
typedef struct {
  PPRListItemVariant variant;
  union {
    TokenType token;
    PPRNonterminal nonterminal;
  } type;
  // value
  union {
      char* string;
      int int_;
      double float_;
  } value;
} PPRListItem;

// /**
//  * @brief Precedence table type.
//  *
//  * The table is of size m*n, where m represents terminal and n represents
//  * token.
//  */
// typedef PrecedentOperation **PrecedentTable;

#endif /* PRECEDENCE_PARSING_H */