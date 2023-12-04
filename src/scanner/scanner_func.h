/**
 * @name Implementation of IFJ23 language compiler
 * @file scanner_func.h
 * @brief header file containing function definitions for scanner
 *
 * @author Denis Milistenfer (xmilis00)
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
 * @brief this functions checks if a word that has been read is a keyword
 * 
 * @param keyword pointer to a word that was scanned
 * @return token for corresponding keyword or TOKEN_IDENTIF 
 * if passed word does not match with any IFJ23 keywords
 */
TokenData is_keyword(char *keyword);

/**
 * @brief this function skips whitespaces in code until first non-whitespace
 * character is read and handles '\n'
 * 
 * @param opt scanner options struct pointer from which code is being read
 * @return true if newline was among whitespaces
 * @return false if newline was not among whitespaces
 */
bool _skip_whitespaces(ScannerOptions *opt);

/**
 * @brief this function removes extra whitespaces in multiline string
 * according to swift multiline indentation
 * 
 * @param buf pointer to scanner buffer struct where string is stored
 * @param count number of whitespaces before closing quotes 
 * (number of whitespaces that needs to be removed)
 */
void _remove_extra_whitespaces(ScannerBuffer *buf, int count);

/**
 * @brief this function frees allocated memmory and sets 
 * scanner options structure to it's initialization state
 * 
 * @param opt scanner options struct pointer containing all necessary data
 * for handling input code IFJ23 
 */
void scanner_opt_free(ScannerOptions *opt);

/**
 * @brief this function initializes all necessary values inside scanner option structure,
 * allocates memmory for input file and stores it
 * 
 * @param opt scanner options struct pointer containing all necessary data
 * for handling input code IFJ23 
 * @param file pointer to input file
 * @return true if all allocations are successfull
 * @return false if allocation error occurs
 */
bool scanner_opt_init(ScannerOptions *opt, FILE *file);

/**
 * @brief this function sets scanner option structure to it's initialization state
 * to allow second run through file
 * 
 * @param opt scanner options struct pointer containing all necessary data
 * for handling input code IFJ23 
 */
void scanner_rewind_file(ScannerOptions *opt);

/**
 * @brief this function copies value of src token to dest token
 * 
 * @param dest pointer to destination token where value is being copied to
 * @param src source token which value is being copied from
 * @return true if everything was successfull
 * @return false if allocation within clone_string() function was not successfull
 */
bool clone_token(TokenData *dest, TokenData src);

/**
 * @brief this function frees allocated memmory for identifier token value or
 * token string value 
 * 
 * @param token token which memmory needs to be freed
 */
void free_token(TokenData token);

/**
 * @brief this function initializes all necessary values inside scanner buffer struct and
 * allocates memmory for buffer
 * 
 * @param buf pointer to scanner buffer structure
 * @return true if allocation was successfull
 * @return false if allocation error occured
 */
bool scanner_buf_init(ScannerBuffer *buf);

/**
 * @brief this function sets all neccesary values inside scanner buffer struct to it's initialization state and
 * frees memmory for buffer
 * 
 * @param buf pointer to scanner buffer structure
 */
void scanner_buf_free(ScannerBuffer *buf);

/**
 * @brief this function inserts character to buffer and reallocates memmory if needed
 * 
 * @param buf pointer to scanner buffer structure
 * @param ch character to be stored
 * @return true if reallocation was succesfull
 * @return false if reallocation error occured
 */
bool scanner_buf_insert(ScannerBuffer *buf, char ch);

/**
 * @brief this function returns next character from input file
 * likewise to fgetc() function
 * 
 * @param opt scanner options struct pointer containing all necessary data
 * for handling input code IFJ23
 * @return ascii value of returned character
 */
int get_next_char(ScannerOptions *opt);

/**
 * @brief main scanner function that contains finite state machine 
 * and handles recognition of various tokens
 * 
 * @param parser_opt parser options struct pointer where token and other neccessary
 *  data is being written to
 * @return true if no lexical or internal error occured and proper token was recognized
 * @return false if lexical or internal error occured
 */
bool get_next_token(ParserOptions *parser_opt);

#endif /* SCANNER_FUNC_H */
