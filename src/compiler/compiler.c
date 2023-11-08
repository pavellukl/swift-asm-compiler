#include "compiler.h"

CompilerReturnValue _get_return_code(RPReturnValue rp_return_value) {
    switch (rp_return_value)
    {
      case RP_OK: return COMP_OK;
      case RP_LEX_ERR: return COMP_LEX_ERR;
      case RP_STX_ERR: return COMP_STX_ERR;
      case RP_DEF_ERR: return COMP_DEF_ERR;
      case RP_FNCALL_ERR: return COMP_FNCALL_ERR;
      case RP_UNDEFVAR_ERR: return COMP_UNDEFVAR_ERR;
      case RP_FNRET_ERR: return COMP_FNRET_ERR;
      case RP_EXPRTYPE_ERR: return COMP_EXPRTYPE_ERR;
      case RP_UNDEFTYPE_ERR: return COMP_UNDEFTYPE_ERR;
      case RP_OTHER_ERR: return COMP_OTHER_ERR;
      case RP_INTER_ERR: return COMP_INTER_ERR;
      default: return COMP_INTER_ERR;
    }
}

CompilerReturnValue compile(FILE *in, FILE *out) {
    assert(in != NULL);
    assert(out != NULL);

    // property init
    ParserOptions parser_opt;
    if(!scanner_opt_init(&parser_opt.sc_opt, in)) return COMP_INTER_ERR;
    parser_opt.out = out;

    // first run
    parse_function_definition(&parser_opt);
    switch (parser_opt.return_code)
    {
      case RP_OK: break;
      default: 
        scanner_opt_free(&parser_opt.sc_opt);
        return _get_return_code(parser_opt.return_code);
    }

    // file rewind
    scanner_rewind_file(&parser_opt.sc_opt);

    // second run
    parse_check_optimize_generate(&parser_opt);

    scanner_opt_free(&parser_opt.sc_opt);
    return _get_return_code(parser_opt.return_code);
}