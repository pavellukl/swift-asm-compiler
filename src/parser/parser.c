#include "parser.h"

bool _next_token(ParserOptions *parser_opt) {
    free_token(parser_opt->token);
    return get_next_token(parser_opt);
}
