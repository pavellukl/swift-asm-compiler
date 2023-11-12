#include "compiler.h"

CompilerReturnValue _get_return_code(RPReturnValue rp_return_value) {
    switch (rp_return_value) {
        case OK:
            return COMP_OK;
        case LEX_ERR:
            return COMP_LEX_ERR;
        case STX_ERR:
            return COMP_STX_ERR;
        case DEF_ERR:
            return COMP_DEF_ERR;
        case FNCALL_ERR:
            return COMP_FNCALL_ERR;
        case UNDEFVAR_ERR:
            return COMP_UNDEFVAR_ERR;
        case FNRET_ERR:
            return COMP_FNRET_ERR;
        case EXPRTYPE_ERR:
            return COMP_EXPRTYPE_ERR;
        case UNDEFTYPE_ERR:
            return COMP_UNDEFTYPE_ERR;
        case OTHER_ERR:
            return COMP_OTHER_ERR;
        case INTER_ERR:
            return COMP_INTER_ERR;
        default:
            return COMP_INTER_ERR;
    }
}

CompilerReturnValue compile(FILE *in, FILE *out) {
    printf("run\n");
    assert(in != NULL);
    assert(out != NULL);

    printf("pass asserts\n");

    // property init
    ParserOptions parser_opt;
    if (!scanner_opt_init(&parser_opt.sc_opt, in)) return COMP_INTER_ERR;
    parser_opt.out = out;

    printf("running compiler\n");

    // first run
    parser_opt.is_first_run = true;
    printf("running compiler2\n");

    parse_function_definition(&parser_opt);
    printf("running compiler3\n");

    switch (parser_opt.return_code) {
        case OK:
            break;
        default:
            scanner_opt_free(&parser_opt.sc_opt);
            return _get_return_code(parser_opt.return_code);
    }

    printf("running second run\n");

    // file rewind
    scanner_rewind_file(&parser_opt.sc_opt);

    // second run
    parser_opt.is_first_run = false;
    parse_check_optimize_generate(&parser_opt);

    printf("second run done\n");

    scanner_opt_free(&parser_opt.sc_opt);
    return _get_return_code(parser_opt.return_code);
}