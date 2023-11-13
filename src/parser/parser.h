#ifndef PARSER_H
#define PARSER_H

#include "../scanner/scanner.h"
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
    bool is_first_run;
    ScannerOptions sc_opt;
    TokenData token;
    FILE *out;
    ListST *symtable;
    ParserReturnCode return_code;
} ParserOptions;

void _next_token(ParserOptions *parser_opt);

#endif /* PARSER_H */