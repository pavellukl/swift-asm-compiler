#include "parser.h"

bool _next_token(ParserOptions *parser_opt) {
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
