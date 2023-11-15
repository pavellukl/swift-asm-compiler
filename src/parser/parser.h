#ifndef PARSER_H
#define PARSER_H

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
    LSTElement new_identif;
    Parameter new_param;
    Type new_type;
} SemanticVariables;

typedef struct {
    bool is_first_run;
    ScannerOptions sc_opt;
    TokenData token;
    FILE *out;
    ListST *symtable;
    ParserReturnCode return_code;
    // for easily passing data between functions in semantic analysis
    SemanticVariables variables;
} ParserOptions;

#include "../scanner/scanner_func.h"

bool _next_token(ParserOptions *parser_opt);

#endif /* PARSER_H */