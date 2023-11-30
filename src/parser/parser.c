#include "parser.h"

bool _next_token(ParserOptions *parser_opt) {
    if (parser_opt->sem_ctx.is_eol_before_required &&
        !parser_opt->token.eol_before /*&&
        parser_opt->token.type != TOKEN_END_OF_FILE*/) {
        parser_opt->return_code = STX_ERR;
        return false;
    }

    parser_opt->sem_ctx.is_eol_before_required = false;

    free_token(parser_opt->token);

    if (!get_next_token(parser_opt)) {
        PRINTF_STDDEBUG("analysis getting token: %d\n", parser_opt->token.type);
        return false;
    };

    PRINTF_STDDEBUG("analysis getting token: %d\n", parser_opt->token.type);

    return true;
}

bool _first_token(ParserOptions *parser_opt) {
    if (!get_next_token(parser_opt)) {
        PRINTF_STDDEBUG("analysis getting token: %d\n", parser_opt->token.type);
        return false;
    };

    PRINTF_STDDEBUG("analysis getting token: %d\n", parser_opt->token.type);
    return true;
}
