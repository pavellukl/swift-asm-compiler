#ifndef PRECEDENCE_PARSER_H
#define PRECEDENCE_PARSER_H

#include "../scanner/scanner.h"
#include "parser.h"
#include "precedence_table.h"

/** @brief Represents an item in the precedence list. */
typedef struct {
  PPListItemType type;
  TokenNumberType num_type;
  union {
      char* string;
      int int_;
      double float_;
      bool bool_;
  } value;
} PPListItem;

bool parse_check_optimize_generate_expression(ParserOptions *parser_opt);

bool precedence_table_init();

#endif /* PRECEDENCE_PARSER_H */