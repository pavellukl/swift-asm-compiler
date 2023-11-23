#include "compiler.h"

CompilerReturnCode _get_return_code(ParserReturnCode rp_return_value) {
    switch (rp_return_value) {
        case OK:
            return COMP_OK;
        case LEX_ERR:
            return COMP_LEX_ERR;
        case STX_ERR:
            return COMP_STX_ERR;
        case DEF_ERR:
            return COMP_DEF_ERR;
        case UNDEFVAR_ERR:
            return COMP_UNDEFVAR_ERR;
        case FNCALL_ERR:
            return COMP_FNCALL_ERR;
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

CompilerReturnCode compile(FILE *in, FILE *out) {
    PRINTF_STDDEBUG("running compiler\n");
    assert(in != NULL);

    // property init
    PRINTF_STDDEBUG("init\n");
    ParserOptions parser_opt;
    if (!scanner_opt_init(&parser_opt.sc_opt, in)) return COMP_INTER_ERR;
    parser_opt.symtable = st_create_list();
    if (parser_opt.symtable == NULL) {
        scanner_opt_free(&parser_opt.sc_opt);
        return COMP_INTER_ERR;
    }

    if (!generation_init(&parser_opt.gen_var)) {
        scanner_opt_free(&parser_opt.sc_opt);
        st_destroy_list(parser_opt.symtable);
        return COMP_INTER_ERR;
    }

    // first run preparations
    PRINTF_STDDEBUG("first run prep\n");
    parser_opt.is_first_run = true;
    if (st_push_scope(parser_opt.symtable, parser_opt.gen_var.scope_n) !=
        E_OK) {
        return COMP_INTER_ERR;
    };

    // first run
    PRINTF_STDDEBUG("first run\n");
    parse_function_definition(&parser_opt);
    if (parser_opt.return_code != OK) {
        st_pop_scope(parser_opt.symtable);
        scanner_opt_free(&parser_opt.sc_opt);
        st_destroy_list(parser_opt.symtable);
        generation_free(parser_opt.gen_var);
        return _get_return_code(parser_opt.return_code);
    }

    // second run preparations
    PRINTF_STDDEBUG("second run prep\n");
    parser_opt.is_first_run = false;
    init_semantic_context(&parser_opt.sem_ctx);
    scanner_rewind_file(&parser_opt.sc_opt);
    if (!add_inbuilt_functions_to_symtable(parser_opt.symtable) ||
        !generate_inbuilt_functions(parser_opt.symtable, parser_opt.gen_var)) {
        st_pop_scope(parser_opt.symtable);
        scanner_opt_free(&parser_opt.sc_opt);
        st_destroy_list(parser_opt.symtable);
        generation_free(parser_opt.gen_var);
        return COMP_INTER_ERR;
    }

    // second run
    PRINTF_STDDEBUG("second run\n");
    parse_check_optimize_generate(&parser_opt);
    if (parser_opt.return_code != OK) {
        st_pop_scope(parser_opt.symtable);
        scanner_opt_free(&parser_opt.sc_opt);
        st_destroy_list(parser_opt.symtable);
        generation_free(parser_opt.gen_var);
        return _get_return_code(parser_opt.return_code);
    }

    // printing to file out
    PRINTF_STDDEBUG("printing to file out\n");
    print_generation_to_file(parser_opt.gen_var, out);

    st_pop_scope(parser_opt.symtable);
    scanner_opt_free(&parser_opt.sc_opt);
    st_destroy_list(parser_opt.symtable);
    generation_free(parser_opt.gen_var);
    return COMP_OK;
}
