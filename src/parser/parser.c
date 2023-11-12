#include "parser.h"

void _next_token(ParserOptions *parser_opt) {
    parser_opt->token = get_next_token(&parser_opt->sc_opt);
}
