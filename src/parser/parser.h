#ifndef PARSER_H
#define PARSER_H

#include "../scanner/scanner.h"

typedef enum { OK, LEX_ERR, STX_ERR, DEF_ERR,
               UNDEFVAR_ERR, FNCALL_ERR, FNRET_ERR,
               EXPRTYPE_ERR, UNDEFTYPE_ERR, OTHER_ERR,
               INTER_ERR } RPReturnValue;

typedef struct {
    bool is_first_run;
    ScannerOptions sc_opt;
    TokenData token;
    FILE *out;
    RPReturnValue return_code;
} ParserOptions;

void _next_token(ParserOptions *parser_opt);

#endif /* PARSER_H */