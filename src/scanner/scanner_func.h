/**
 * @file scanner_func.h
 * @brief TODO: file header
 * 
 * @author your name (you@domain.com)
 */

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
#include "../helpers/utils.h"

/**
 * @brief TODO:
 * 
 * @param keyword 
 * @return TokenData 
 */
TokenData is_keyword(char *keyword);

/**
 * @brief TODO:
 * 
 * @param opt 
 * @return true 
 * @return false 
 */
bool _skip_whitespaces(ScannerOptions *opt);

/**
 * @brief TODO:
 * 
 * @param buf 
 * @param count 
 */
void _remove_extra_whitespaces(ScannerBuffer *buf, int count);

/**
 * @brief TODO:
 * 
 * @param opt 
 */
void scanner_opt_free(ScannerOptions *opt);

/**
 * @brief TODO:
 * 
 * @param opt 
 * @param file 
 * @return true 
 * @return false 
 */
bool scanner_opt_init(ScannerOptions *opt, FILE *file);

/**
 * @brief TODO:
 * 
 * @param opt 
 */
void scanner_rewind_file(ScannerOptions *opt);

/**
 * @brief TODO:
 * 
 * @param dest 
 * @param src 
 * @return true 
 * @return false 
 */
bool clone_token(TokenData *dest, TokenData src);

/**
 * @brief TODO:
 * 
 * @param token 
 */
void free_token(TokenData token);

/**
 * @brief TODO:
 * 
 * @param buf 
 * @return true 
 * @return false 
 */
bool scanner_buf_init(ScannerBuffer *buf);

/**
 * @brief TODO:
 * 
 * @param buf 
 */
void scanner_buf_free(ScannerBuffer *buf);

/**
 * @brief TODO:
 * 
 * @param buf 
 * @param ch 
 * @return true 
 * @return false 
 */
bool scanner_buf_insert(ScannerBuffer *buf, char ch);

/**
 * @brief TODO:
 * 
 * @param opt 
 * @return int 
 */
int get_next_char(ScannerOptions *opt);

/**
 * @brief TODO:
 * 
 * @param parser_opt 
 * @return true 
 * @return false 
 */
bool get_next_token(ParserOptions *parser_opt);

#endif /* SCANNER_FUNC_H */
