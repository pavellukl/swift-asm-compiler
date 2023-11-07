#ifndef PARSER_H
#define PARSER_H

#include "../scanner/scanner.h"

typedef enum { RP_OK, RP_LEX_ERR, RP_STX_ERR, RP_DEF_ERR,
               RP_UNDEFVAR_ERR, RP_FNCALL_ERR, RP_FNRET_ERR,
               RP_EXPRTYPE_ERR, RP_UNDEFTYPE_ERR, RP_OTHER_ERR,
               RP_INTER_ERR } RPReturnValue;

typedef struct {
    ScannerOptions sc_opt;
    TokenData token;
    FILE *out;
    RPReturnValue return_code;
} ParserOptions;

#endif /* PARSER_H */