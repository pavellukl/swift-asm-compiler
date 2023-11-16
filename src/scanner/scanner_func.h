#ifndef SCANNER_FUNC_H
#define SCANNER_FUNC_H

#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../debug.h"
#include "scanner_types.h"
#include "../parser/parser.h"


TokenData is_keyword(char *keyword);

bool _skip_whitespaces(ScannerOptions *opt);

void scanner_opt_free(ScannerOptions *opt);

bool scanner_opt_init(ScannerOptions *opt, FILE *file);

void scanner_rewind_file(ScannerOptions *opt);

void free_token(TokenData token);

int get_next_char(ScannerOptions *opt);

bool get_next_token(ParserOptions *parser_opt);

#endif /* SCANNER_FUNC_H */
