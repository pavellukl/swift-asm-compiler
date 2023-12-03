/**
 * @file scanner.c
 * @brief contains implemenation of the scanner (get_next_token and some other
 * service functions)
 *
 * @author Denis Milistenfer (xmilis00)
 * @author Pavel Lukl (xluklp00)
 */

#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "scanner_func.h"

TokenData is_keyword(char* keyword) {
    if (strcmp(keyword, "Double") == 0) {
        return (TokenData){
            .type = TOKEN_KEYWORD_DOUBLE,
        };
    }
    if (strcmp(keyword, "Double?") == 0) {
        return (TokenData){.type = TOKEN_KEYWORD_DOUBLE_NIL};
    }
    if (strcmp(keyword, "else") == 0) {
        return (TokenData){.type = TOKEN_KEYWORD_ELSE};
    }
    if (strcmp(keyword, "func") == 0) {
        return (TokenData){.type = TOKEN_KEYWORD_FUNC};
    }
    if (strcmp(keyword, "if") == 0) {
        return (TokenData){.type = TOKEN_KEYWORD_IF};
    }
    if (strcmp(keyword, "Int") == 0) {
        return (TokenData){.type = TOKEN_KEYWORD_INT};
    }
    if (strcmp(keyword, "Int?") == 0) {
        return (TokenData){.type = TOKEN_KEYWORD_INT_NIL};
    }
    if (strcmp(keyword, "let") == 0) {
        return (TokenData){.type = TOKEN_KEYWORD_LET};
    }
    if (strcmp(keyword, "nil") == 0) {
        return (TokenData){.type = TOKEN_KEYWORD_NIL};
    }
    if (strcmp(keyword, "return") == 0) {
        return (TokenData){.type = TOKEN_KEYWORD_RETURN};
    }
    if (strcmp(keyword, "String") == 0) {
        return (TokenData){.type = TOKEN_KEYWORD_STRING};
    }
    if (strcmp(keyword, "String?") == 0) {
        return (TokenData){.type = TOKEN_KEYWORD_STRING_NIL};
    }
    if (strcmp(keyword, "var") == 0) {
        return (TokenData){.type = TOKEN_KEYWORD_VAR};
    }
    if (strcmp(keyword, "while") == 0) {
        return (TokenData){.type = TOKEN_KEYWORD_WHILE};
    }
    if (strcmp(keyword, "true") == 0) {
        return (TokenData){.type = TOKEN_BOOL, .value.boolean = true};
    }
    if (strcmp(keyword, "false") == 0) {
        return (TokenData){.type = TOKEN_BOOL, .value.boolean = false};
    }
    if (strcmp(keyword, "Bool") == 0) {
        return (TokenData){.type = TOKEN_KEYWORD_BOOL};
    }
    if (strcmp(keyword, "Bool?") == 0) {
        return (TokenData){.type = TOKEN_KEYWORD_BOOL_NIL};
    }

    return (TokenData){.type = TOKEN_IDENTIF};
}

bool _skip_whitespaces(ScannerOptions* opt) {
    char ch;
    bool newline_found = false;

    while (opt->i < opt->size) {
        ch = get_next_char(opt);
        if (ch == '\n') {
            opt->line_counter++;
            newline_found = true;
        } else if (!isspace(ch)) {
            opt->i--;
            break;
        }
    }

    return newline_found;
}

void _remove_extra_whitespaces(ScannerBuffer *buf, int count) {
    int read_index = 0; 
    int write_index = 0;
    int skip_count;
    bool is_newline = true;

    while (read_index < buf->i) {
        if (is_newline) {
            skip_count = count;
            is_newline = false;
        }

        if (buf->buffer[read_index] == '\n') {
            is_newline = true;
        }

        if (skip_count > 0 && (buf->buffer[read_index] == ' ' || buf->buffer[read_index] == '\t')) {
            skip_count--;
        } else {
            buf->buffer[write_index++] = buf->buffer[read_index];
        }

        read_index++;
    }

    buf->buffer[--write_index] = '\0';
    buf->i = write_index;
}

bool scanner_opt_init(ScannerOptions* opt, FILE* file) {
    int capacity = 128;
    opt->size = 0;

    opt->file = malloc(sizeof *opt->file * capacity);
    if (opt->file == NULL) return false;

    int c;
    while ((c = fgetc(file)) != EOF) {
        if (opt->size == capacity) {
            capacity *= 2;
            opt->file = realloc(opt->file, sizeof *opt->file * capacity);
            if (opt->file == NULL) return false;
        }

        opt->file[opt->size] = c;
        opt->size++;
    }

    opt->i = 0;
    opt->line_counter = 0;
    opt->id_counter = 0;

    opt->returned_tokens = stack_token_data_init();

    return true;
}

void scanner_opt_free(ScannerOptions* opt) {
    free(opt->file);
    stack_token_data_free(opt->returned_tokens);

    opt->file = NULL;
    opt->returned_tokens = NULL;
    opt->size = 0;
    opt->line_counter = 0;
    opt->id_counter = 0;
}

void scanner_rewind_file(ScannerOptions* opt) {
    opt->i = 0;
    opt->line_counter = 0;
    opt->id_counter = 0;
}

bool clone_token(TokenData *dest, TokenData src) {
    *dest = src;
    if (src.type == TOKEN_IDENTIF || src.type == TOKEN_STRING) {
        return clone_string(&dest->value.string, src.value.string);
    }
    return true;
}

void free_token(TokenData token) {
    if (token.type == TOKEN_IDENTIF || token.type == TOKEN_STRING) {
        free(token.value.string);
    }
}

bool scanner_buf_init(ScannerBuffer* buf) {
    int capacity = 128;

    buf->buffer = malloc(sizeof(char) * capacity);
    if (buf->buffer == NULL) return false;

    buf->i = 0;
    buf->size = capacity;

    return true;
}

void scanner_buf_free(ScannerBuffer* buf) {
    free(buf->buffer);

    buf->buffer = NULL;
    buf->i = 0;
    buf->size = 0;
}

bool scanner_buf_insert(ScannerBuffer* buf, char ch) {
    if (buf->i == buf->size) {
        int new_capacity = buf->size * 2;
        buf->buffer = realloc(buf->buffer, sizeof(char) * new_capacity);
        if (buf->buffer == NULL) return false;

        buf->size = new_capacity;
    }
    buf->buffer[buf->i++] = ch;
    return true;
}

int get_next_char(ScannerOptions* opt) {
    PRINTF_STDDEBUG("INDEX: %d/%d\n", opt->i, opt->size);
    if (opt->i < opt->size) {
        PRINTF_STDDEBUG("RETURNING CHAR: %c(%d)\n", opt->file[opt->i],
                        opt->file[opt->i]);

        return opt->file[opt->i++];
    } else {
        PRINTF_STDDEBUG("RETURNING CHAR: EOF\n");

        opt->i++;
        return EOF;
    }
}

bool get_next_token(ParserOptions* parser_opt) {
    PRINTF_STDDEBUG("-----------------CALL\n");
    TokenData token;
    AutomatState current_state = START;

    if (stack_token_data_is_empty(parser_opt->sc_opt.returned_tokens) == false) {
        stack_token_data_pop(parser_opt->sc_opt.returned_tokens, &token);
        parser_opt->token = token;
        return true;
    }

    ScannerBuffer b_buffer;

    char unicode_buf[9];
    int unicode_index = 0;

    char current_char = get_next_char(&parser_opt->sc_opt);
    char next_char;

    int block_comment_count = 0;

    bool new_line_online = false;
    int whitespace_count = 0;

    bool new_line_before_token = false;

    while (true) {
        PRINTF_STDDEBUG("STATE: %d, working with CHAR: %c(%d)\n",
                        current_state, current_char, current_char);

        switch (current_state) {
            case START:
                switch (current_char) {
                    case '/':
                        current_state = DIV;
                        break;
                    case '+':
                        current_state = ADD;
                        break;
                    case '-':
                        current_state = SUB;
                        break;

                    case '*':
                        current_state = MUL;
                        break;

                    case '!':
                        current_state = EXCL_MARK;
                        break;

                    case '=':
                        current_state = ASSIGN;
                        break;

                    case '>':
                        current_state = GREATER;
                        break;

                    case '<':
                        current_state = LESSER;
                        break;

                    case '(':
                        current_state = L_BRACKET;
                        break;

                    case ')':
                        current_state = R_BRACKET;
                        break;

                    case '{':
                        current_state = L_CRLY_BRACKET;
                        break;

                    case '}':
                        current_state = R_CRLY_BRACKET;
                        break;

                    case ':':
                        current_state = COLON;
                        break;

                    case ',':
                        current_state = COMA;
                        break;

                    case '?':
                        current_state = QUESTION_MARK;
                        break;

                    case '_':
                        current_state = UNDERSCORE;
                        break;

                    case '|':
                        current_state = OR;
                        break;

                    case '&':
                        current_state = AND;
                        break;

                    case ' ':
                    case '\0':
                    case '\t':
                    case '\v':
                    case '\f':
                    case '\r':
                        current_state = WHITE_SPACE;
                        break;

                    case '\n':
                        new_line_before_token = true;
                        parser_opt->sc_opt.line_counter++;
                        current_char = get_next_char(&parser_opt->sc_opt);
                        current_state = START;
                        break;

                    case EOF:
                        current_state = END_OF_FILE;
                        break;

                    case '"':
                        scanner_buf_init(&b_buffer);
                        current_char = get_next_char(&parser_opt->sc_opt);
                        current_state = STRING;
                        break;

                    default:
                        if (isdigit(current_char)) {
                            scanner_buf_init(&b_buffer);
                            current_state = NUM;
                        } else if (isalpha(current_char)) {
                            current_state = IDENTIF;
                        } else {
                            parser_opt->return_code = LEX_ERR;
                            return false;
                        }
                        break;
                }
                break;

            case DIV:
                next_char = get_next_char(&parser_opt->sc_opt);
                if (next_char == '/') {
                    current_state = LINE_COMMENT;
                } else if (next_char == '*') {
                    current_state = BLOCK_COMMENT;
                    block_comment_count++;
                } else {
                    token.type = TOKEN_DIV;
                    token.line_index = parser_opt->sc_opt.line_counter;
                    token.eol_before = new_line_before_token;
                    token.id = parser_opt->sc_opt.id_counter++;
                    parser_opt->sc_opt.i--;
                    parser_opt->token = token;
                    return true;
                }
                break;

            case LINE_COMMENT:
                while (((next_char = get_next_char(&parser_opt->sc_opt)) !=
                        '\n') &&
                       (next_char != EOF)) {
                    // skipping characters
                }

                if (next_char == EOF) {
                    parser_opt->sc_opt.i--;
                    current_state = END_OF_FILE;
                    break;
                } else {
                    new_line_before_token = true;
                }

                current_state = WHITE_SPACE;
                break;

            case BLOCK_COMMENT:
                next_char = get_next_char(&parser_opt->sc_opt);
                while (next_char != EOF) {
                    if (next_char == '*') {
                        current_state = BLOCK_COMMENT_END;
                    } else if (next_char == '/') {
                        char next_char_peek =
                            get_next_char(&parser_opt->sc_opt);
                        if (next_char_peek == '*') {
                            block_comment_count++;
                        } else {
                            parser_opt->sc_opt.i--;
                        }
                    }
                    if (current_state == BLOCK_COMMENT) {
                        next_char = get_next_char(&parser_opt->sc_opt);
                    } else {
                        break;
                    }
                }
                if (next_char == EOF && block_comment_count > 0) {
                    // LEX ERROR comment not closed before eof
                    parser_opt->return_code = LEX_ERR;
                    return false;
                }
                break;

            case BLOCK_COMMENT_END:
                next_char = get_next_char(&parser_opt->sc_opt);
                if (next_char == '/') {
                    block_comment_count--;
                    if (block_comment_count == 0) {
                        current_state = WHITE_SPACE;
                    } else {
                        current_state = BLOCK_COMMENT;
                        next_char = get_next_char(&parser_opt->sc_opt);
                    }
                } else {
                    if (next_char != '*') {
                        current_state = BLOCK_COMMENT;
                    }
                }
                break;

            case ADD:
                token.type = TOKEN_ADD;
                token.line_index = parser_opt->sc_opt.line_counter;
                token.eol_before = new_line_before_token;
                token.id = parser_opt->sc_opt.id_counter++;
                parser_opt->token = token;
                return true;

            case SUB:
                next_char = get_next_char(&parser_opt->sc_opt);
                if (next_char == '>') {
                    current_state = ARROW;
                } else {
                    token.type = TOKEN_SUB;
                    token.line_index = parser_opt->sc_opt.line_counter;
                    token.eol_before = new_line_before_token;
                    token.id = parser_opt->sc_opt.id_counter++;
                    parser_opt->sc_opt.i--;
                    parser_opt->token = token;
                    return true;
                }
                break;

            case ARROW:
                token.type = TOKEN_ARROW;
                token.line_index = parser_opt->sc_opt.line_counter;
                token.eol_before = new_line_before_token;
                token.id = parser_opt->sc_opt.id_counter++;
                parser_opt->token = token;
                return true;

            case MUL:
                token.type = TOKEN_MUL;
                token.line_index = parser_opt->sc_opt.line_counter;
                token.eol_before = new_line_before_token;
                token.id = parser_opt->sc_opt.id_counter++;
                parser_opt->token = token;
                return true;

            case EXCL_MARK:
                next_char = get_next_char(&parser_opt->sc_opt);
                if (next_char == '=') {
                    current_state = NOT_EQUAL;
                } else if (isalpha(next_char) || next_char == '_' ||
                          next_char == '(') {
                    token.type = TOKEN_NOT;
                    token.line_index = parser_opt->sc_opt.line_counter;
                    token.eol_before = new_line_before_token;
                    token.id = parser_opt->sc_opt.id_counter++;
                    parser_opt->sc_opt.i--;
                    parser_opt->token = token;
                    return true;
                } else {
                    token.type = TOKEN_EXCL_MARK;
                    token.line_index = parser_opt->sc_opt.line_counter;
                    token.eol_before = new_line_before_token;
                    token.id = parser_opt->sc_opt.id_counter++;
                    parser_opt->sc_opt.i--;
                    parser_opt->token = token;
                    return true;
                }
                break;

            case NOT_EQUAL:
                token.type = TOKEN_NOT_EQUAL;
                token.line_index = parser_opt->sc_opt.line_counter;
                token.eol_before = new_line_before_token;
                token.id = parser_opt->sc_opt.id_counter++;
                parser_opt->token = token;
                return true;

            case ASSIGN:
                next_char = get_next_char(&parser_opt->sc_opt);
                if (next_char == '=') {
                    current_state = EQUAL;
                } else {
                    token.type = TOKEN_ASSIGN;
                    token.line_index = parser_opt->sc_opt.line_counter;
                    token.eol_before = new_line_before_token;
                    token.id = parser_opt->sc_opt.id_counter++;
                    parser_opt->sc_opt.i--;
                    parser_opt->token = token;
                    return true;
                }
                break;

            case EQUAL:
                token.type = TOKEN_EQUAL;
                token.line_index = parser_opt->sc_opt.line_counter;
                token.eol_before = new_line_before_token;
                token.id = parser_opt->sc_opt.id_counter++;
                parser_opt->token = token;
                return true;

            case GREATER:
                next_char = get_next_char(&parser_opt->sc_opt);
                if (next_char == '=') {
                    current_state = GREATER_EQUAL;
                } else {
                    token.type = TOKEN_GREATER;
                    token.line_index = parser_opt->sc_opt.line_counter;
                    token.eol_before = new_line_before_token;
                    token.id = parser_opt->sc_opt.id_counter++;
                    parser_opt->sc_opt.i--;
                    parser_opt->token = token;
                    return true;
                }
                break;

            case LESSER:
                next_char = get_next_char(&parser_opt->sc_opt);
                if (next_char == '=') {
                    current_state = LESSER_EQUAL;
                } else {
                    token.type = TOKEN_LESSER;
                    token.line_index = parser_opt->sc_opt.line_counter;
                    token.eol_before = new_line_before_token;
                    token.id = parser_opt->sc_opt.id_counter++;
                    parser_opt->sc_opt.i--;
                    parser_opt->token = token;
                    return true;
                }
                break;

            case GREATER_EQUAL:
                token.type = TOKEN_GREATER_EQUAL;
                token.line_index = parser_opt->sc_opt.line_counter;
                token.eol_before = new_line_before_token;
                token.id = parser_opt->sc_opt.id_counter++;
                parser_opt->token = token;
                return true;

            case LESSER_EQUAL:
                token.type = TOKEN_LESSER_EQUAL;
                token.line_index = parser_opt->sc_opt.line_counter;
                token.eol_before = new_line_before_token;
                token.id = parser_opt->sc_opt.id_counter++;
                parser_opt->token = token;
                return true;

            case L_BRACKET:
                token.type = TOKEN_L_BRACKET;
                token.line_index = parser_opt->sc_opt.line_counter;
                token.eol_before = new_line_before_token;
                token.id = parser_opt->sc_opt.id_counter++;
                parser_opt->token = token;
                return true;

            case R_BRACKET:
                token.type = TOKEN_R_BRACKET;
                token.line_index = parser_opt->sc_opt.line_counter;
                token.eol_before = new_line_before_token;
                token.id = parser_opt->sc_opt.id_counter++;
                parser_opt->token = token;
                return true;

            case L_CRLY_BRACKET:
                token.type = TOKEN_L_CRLY_BRACKET;
                token.line_index = parser_opt->sc_opt.line_counter;
                token.eol_before = new_line_before_token;
                token.id = parser_opt->sc_opt.id_counter++;
                parser_opt->token = token;
                return true;

            case R_CRLY_BRACKET:
                token.type = TOKEN_R_CRLY_BRACKET;
                token.line_index = parser_opt->sc_opt.line_counter;
                token.eol_before = new_line_before_token;
                token.id = parser_opt->sc_opt.id_counter++;
                parser_opt->token = token;
                return true;

            case COLON:
                token.type = TOKEN_COLON;
                token.line_index = parser_opt->sc_opt.line_counter;
                token.eol_before = new_line_before_token;
                token.id = parser_opt->sc_opt.id_counter++;
                parser_opt->token = token;
                return true;

            case COMA:
                token.type = TOKEN_COMA;
                token.line_index = parser_opt->sc_opt.line_counter;
                token.eol_before = new_line_before_token;
                token.id = parser_opt->sc_opt.id_counter++;
                parser_opt->token = token;
                return true;

            case UNDERSCORE:
                next_char = get_next_char(&parser_opt->sc_opt);
                if (next_char == ' ' || next_char == ':') {
                    token.type = TOKEN_UNDERSCORE;
                    token.line_index = parser_opt->sc_opt.line_counter;
                    token.eol_before = new_line_before_token;
                    token.id = parser_opt->sc_opt.id_counter++;
                    parser_opt->sc_opt.i--;
                    parser_opt->token = token;
                    return true;
                } else {
                    parser_opt->sc_opt.i--;
                    current_state = IDENTIF;
                }
                break;

            case IDENTIF:
                scanner_buf_init(&b_buffer);
                while (isalpha(current_char) || current_char == '_' ||
                       isdigit(current_char)) {
                    if (!scanner_buf_insert(&b_buffer, current_char)) {
                        scanner_buf_free(&b_buffer);
                        parser_opt->return_code = INTER_ERR;
                        return false;
                    }
                    current_char = get_next_char(&parser_opt->sc_opt);
                }
                b_buffer.buffer[b_buffer.i] = '\0';

                if (current_char == '?') {
                    if (!scanner_buf_insert(&b_buffer, current_char)) {
                        scanner_buf_free(&b_buffer);
                        parser_opt->return_code = INTER_ERR;
                        return false;
                    }
                    b_buffer.buffer[b_buffer.i] = '\0';
                    token = is_keyword(b_buffer.buffer);
                    if (token.type != TOKEN_IDENTIF) {
                        scanner_buf_free(&b_buffer);
                        token.line_index = parser_opt->sc_opt.line_counter;
                        token.eol_before = new_line_before_token;
                        token.id = parser_opt->sc_opt.id_counter++;
                        parser_opt->token = token;
                        return true;
                    } else {
                        // ERROR: identifier (not keyword) with '?' suffix
                        scanner_buf_free(&b_buffer);
                        parser_opt->return_code = LEX_ERR;
                        return false;
                    }
                }

                parser_opt->sc_opt.i--;
                token = is_keyword(b_buffer.buffer);
                if (token.type != TOKEN_IDENTIF) {
                    scanner_buf_free(&b_buffer);
                    token.line_index = parser_opt->sc_opt.line_counter;
                    token.eol_before = new_line_before_token;
                    token.id = parser_opt->sc_opt.id_counter++;
                    parser_opt->token = token;
                    return true;
                } else {
                    token.value.string =
                        malloc(sizeof(char) * (b_buffer.i + 1));
                    if (token.value.string == NULL) {
                        scanner_buf_free(&b_buffer);
                        parser_opt->return_code = INTER_ERR;
                        return false;
                    }
                    strcpy(token.value.string, b_buffer.buffer);
                    scanner_buf_free(&b_buffer);
                    token.line_index = parser_opt->sc_opt.line_counter;
                    token.eol_before = new_line_before_token;
                    token.id = parser_opt->sc_opt.id_counter++;
                    parser_opt->token = token;
                    return true;
                }
                break;

            case NUM:  // Whole part
                if (isdigit(current_char)) {
                    if (!scanner_buf_insert(&b_buffer, current_char)) {
                        scanner_buf_free(&b_buffer);
                        parser_opt->return_code = INTER_ERR;
                        return false;
                    }
                    current_char = get_next_char(&parser_opt->sc_opt);
                } else if (current_char == '.') {
                    if (!scanner_buf_insert(&b_buffer, current_char)) {
                        scanner_buf_free(&b_buffer);
                        parser_opt->return_code = INTER_ERR;
                        return false;
                    }
                    current_state = DECIMAL_POINT;
                    current_char = get_next_char(&parser_opt->sc_opt);
                } else if (current_char == 'e' || current_char == 'E') {
                    if (!scanner_buf_insert(&b_buffer, current_char)) {
                        scanner_buf_free(&b_buffer);
                        parser_opt->return_code = INTER_ERR;
                        return false;
                    }
                    current_state = EXP_MARK;
                    current_char = get_next_char(&parser_opt->sc_opt);
                } else {
                    b_buffer.buffer[b_buffer.i] = '\0';
                    parser_opt->sc_opt.i--;
                    token.type = TOKEN_INT;
                    token.line_index = parser_opt->sc_opt.line_counter;
                    token.value.int_value = strtol(b_buffer.buffer, NULL, 10);
                    scanner_buf_free(&b_buffer);
                    token.eol_before = new_line_before_token;
                    token.id = parser_opt->sc_opt.id_counter++;
                    parser_opt->token = token;
                    return true;
                }
                break;

            case DECIMAL_POINT:  // Decimal point
                if (isdigit(current_char)) {
                    if (!scanner_buf_insert(&b_buffer, current_char)) {
                        scanner_buf_free(&b_buffer);
                        parser_opt->return_code = INTER_ERR;
                        return false;
                    }
                    current_state = DECIMAL_VALUE;
                    current_char = get_next_char(&parser_opt->sc_opt);
                } else {
                    // Error: Decimal point not followed by digits
                    scanner_buf_free(&b_buffer);
                    parser_opt->return_code = LEX_ERR;
                    return false;
                }
                break;

            case DECIMAL_VALUE:  // Decimal part
                if (isdigit(current_char)) {
                    if (!scanner_buf_insert(&b_buffer, current_char)) {
                        scanner_buf_free(&b_buffer);
                        parser_opt->return_code = INTER_ERR;
                        return false;
                    }
                    current_char = get_next_char(&parser_opt->sc_opt);
                } else if (current_char == 'e' || current_char == 'E') {
                    if (!scanner_buf_insert(&b_buffer, current_char)) {
                        scanner_buf_free(&b_buffer);
                        parser_opt->return_code = INTER_ERR;
                        return false;
                    }
                    current_state = EXP_MARK;
                    current_char = get_next_char(&parser_opt->sc_opt);
                } else {
                    b_buffer.buffer[b_buffer.i] = '\0';
                    parser_opt->sc_opt.i--;
                    token.type = TOKEN_FLOAT;
                    token.line_index = parser_opt->sc_opt.line_counter;
                    token.value.float_value = strtod(b_buffer.buffer, NULL);
                    scanner_buf_free(&b_buffer);
                    token.eol_before = new_line_before_token;
                    token.id = parser_opt->sc_opt.id_counter++;
                    parser_opt->token = token;
                    return true;
                }
                break;

            case EXP_MARK:  // Exponent marker e/E
                if (current_char == '+' || current_char == '-') {
                    if (!scanner_buf_insert(&b_buffer, current_char)) {
                        scanner_buf_free(&b_buffer);
                        parser_opt->return_code = INTER_ERR;
                        return false;
                    }
                    current_state = EXP_SIGN;
                    current_char = get_next_char(&parser_opt->sc_opt);
                } else if (isdigit(current_char)) {
                    if (!scanner_buf_insert(&b_buffer, current_char)) {
                        scanner_buf_free(&b_buffer);
                        parser_opt->return_code = INTER_ERR;
                        return false;
                    }
                    current_state = EXPONENT;
                    current_char = get_next_char(&parser_opt->sc_opt);
                } else {
                    // Error: Exponent marker not followed by sign or digits
                    scanner_buf_free(&b_buffer);
                    parser_opt->return_code = LEX_ERR;
                    return false;
                }
                break;

            case EXP_SIGN:  // Sign after exponent marker +/-
                if (isdigit(current_char)) {
                    if (!scanner_buf_insert(&b_buffer, current_char)) {
                        scanner_buf_free(&b_buffer);
                        parser_opt->return_code = INTER_ERR;
                        return false;
                    }
                    current_state = EXPONENT;
                    current_char = get_next_char(&parser_opt->sc_opt);
                } else {
                    // Error: Exponent sign not followed by digits
                    scanner_buf_free(&b_buffer);
                    parser_opt->return_code = LEX_ERR;
                    return false;
                }
                break;

            case EXPONENT:  // Exponent digits
                if (isdigit(current_char)) {
                    if (!scanner_buf_insert(&b_buffer, current_char)) {
                        scanner_buf_free(&b_buffer);
                        parser_opt->return_code = INTER_ERR;
                        return false;
                    }
                    current_char = get_next_char(&parser_opt->sc_opt);
                } else {
                    b_buffer.buffer[b_buffer.i] = '\0';
                    parser_opt->sc_opt.i--;
                    token.type = TOKEN_FLOAT;
                    token.line_index = parser_opt->sc_opt.line_counter;
                    token.value.float_value = strtod(b_buffer.buffer, NULL);
                    scanner_buf_free(&b_buffer);
                    token.eol_before = new_line_before_token;
                    token.id = parser_opt->sc_opt.id_counter++;
                    parser_opt->token = token;
                    return true;
                }
                break;

            case STRING:
                if (current_char == '"') {
                    next_char = get_next_char(&parser_opt->sc_opt);

                    if (next_char == '"') {
                        next_char = get_next_char(&parser_opt->sc_opt);
                        if (next_char == '\n') {
                            current_state = MULTILINE_STRING;
                        } else {
                            scanner_buf_free(&b_buffer);
                            parser_opt->return_code = LEX_ERR;
                            return false;
                        }

                    } else {
                        parser_opt->sc_opt.i--;
                        current_state = STRING_END;
                    }
                } else if (current_char == '\\') {
                    current_state = ESCAPE_SEQUENCE;
                } else if (current_char == '\n') {
                    scanner_buf_free(&b_buffer);
                    parser_opt->return_code = LEX_ERR;
                    return false;
                } else {
                    if (!scanner_buf_insert(&b_buffer, current_char)) {
                        scanner_buf_free(&b_buffer);
                        parser_opt->return_code = INTER_ERR;
                        return false;
                    }
                }
                current_char = get_next_char(&parser_opt->sc_opt);
                break;

            case MULTILINE_STRING:
                if (current_char == '"') {
                    current_state = MULTILINE_QUOTE_TWO;
                } else {
                    if (current_char == '\n') {
                        new_line_online = true;
                        parser_opt->sc_opt.line_counter++;
                    } else if (!isspace(current_char)){
                        new_line_online = false;
                        whitespace_count = 0;
                    } else if (isspace(current_char)){
                        whitespace_count++;
                    }
                    if (!scanner_buf_insert(&b_buffer, '"')) {
                        scanner_buf_free(&b_buffer);
                        parser_opt->return_code = INTER_ERR;
                        return false;
                    }
                    parser_opt->sc_opt.i--;
                    current_state = MULTILINE_STRING;
                }
                current_char = get_next_char(&parser_opt->sc_opt);
                break;
            
            case MULTILINE_QUOTE_ONE:
                if (current_char == '"') {
                    current_state = MULTILINE_QUOTE_TWO;
                } else {
                    if (current_char == '\n') {
                        new_line_online = true;
                        parser_opt->sc_opt.line_counter++;
                    } else if (!isspace(current_char)){
                        new_line_online = false;
                        whitespace_count = 0;
                    } else if (isspace(current_char)){
                        whitespace_count++;
                    }
                    if (!scanner_buf_insert(&b_buffer, '"')) {
                        scanner_buf_free(&b_buffer);
                        parser_opt->return_code = INTER_ERR;
                        return false;
                    }
                    parser_opt->sc_opt.i--;
                    current_state = MULTILINE_STRING;
                }
                current_char = get_next_char(&parser_opt->sc_opt);
                break;
            
            case MULTILINE_QUOTE_TWO:
                if (current_char == '"') {
                    if (new_line_online == true){
                        b_buffer.i--;
                        parser_opt->sc_opt.line_counter++;
                        if (whitespace_count > 0){
                            _remove_extra_whitespaces(&b_buffer, whitespace_count);
                        }
                        current_state = STRING_END;
                    } else {
                        //not a \n before closing """
                        scanner_buf_free(&b_buffer);
                        parser_opt->return_code = LEX_ERR;
                        return false;
                    }
                } else {
                    if (current_char == '\n') {
                        new_line_online = true;
                        parser_opt->sc_opt.line_counter++;
                    } else if (!isspace(current_char)){
                        new_line_online = false;
                        whitespace_count = 0;
                    } else if (isspace(current_char)){
                        whitespace_count++;
                    }
                    if (!scanner_buf_insert(&b_buffer, '"')) {
                        scanner_buf_free(&b_buffer);
                        parser_opt->return_code = INTER_ERR;
                        return false;
                    }
                    if (!scanner_buf_insert(&b_buffer, '"')) {
                        scanner_buf_free(&b_buffer);
                        parser_opt->return_code = INTER_ERR;
                        return false;
                    }
                    parser_opt->sc_opt.i--;
                    current_state = MULTILINE_STRING;
                }
                current_char = get_next_char(&parser_opt->sc_opt);
                break;

            case ESCAPE_SEQUENCE:
                switch (current_char) {
                    case '\\':
                        if (!scanner_buf_insert(&b_buffer, '\\')) {
                            scanner_buf_free(&b_buffer);
                            parser_opt->return_code = INTER_ERR;
                            return false;
                        }
                        current_state = STRING;
                        break;
                    case '"':
                        if (!scanner_buf_insert(&b_buffer, '"')) {
                            scanner_buf_free(&b_buffer);
                            parser_opt->return_code = INTER_ERR;
                            return false;
                        }
                        current_state = STRING;
                        break;
                    case '\'':
                        if (!scanner_buf_insert(&b_buffer, '\'')) {
                            scanner_buf_free(&b_buffer);
                            parser_opt->return_code = INTER_ERR;
                            return false;
                        }
                        current_state = STRING;
                        break;
                    case '0':
                        if (!scanner_buf_insert(&b_buffer, '\0')) {
                            scanner_buf_free(&b_buffer);
                            parser_opt->return_code = INTER_ERR;
                            return false;
                        }
                        current_state = STRING;
                        break;
                    case 'n':
                        if (!scanner_buf_insert(&b_buffer, '\n')) {
                            scanner_buf_free(&b_buffer);
                            parser_opt->return_code = INTER_ERR;
                            return false;
                        }
                        current_state = STRING;
                        break;
                    case 'r':
                        if (!scanner_buf_insert(&b_buffer, '\r')) {
                            scanner_buf_free(&b_buffer);
                            parser_opt->return_code = INTER_ERR;
                            return false;
                        }
                        current_state = STRING;
                        break;
                    case 't':
                        if (!scanner_buf_insert(&b_buffer, '\t')) {
                            scanner_buf_free(&b_buffer);
                            parser_opt->return_code = INTER_ERR;
                            return false;
                        }
                        current_state = STRING;
                        break;
                    case 'u':
                        current_state = UNICODE_SEQUENCE;
                        break;
                    default:
                        // ERROR: invalid escape sequence
                        scanner_buf_free(&b_buffer);
                        parser_opt->return_code = LEX_ERR;
                        return false;
                }
                current_char = get_next_char(&parser_opt->sc_opt);
                break;

            case UNICODE_SEQUENCE:
                if (current_char == '{') {
                    current_state = UNICODE_VALUE;
                } else {
                    // ERROR: invalid unicode sequence
                    scanner_buf_free(&b_buffer);
                    parser_opt->return_code = LEX_ERR;
                    return false;
                }
                current_char = get_next_char(&parser_opt->sc_opt);
                break;

            case UNICODE_VALUE:
                if (isxdigit(current_char)) {
                    while (isxdigit(current_char) && unicode_index < 8) {
                        unicode_buf[unicode_index++] = current_char;
                        current_char = get_next_char(&parser_opt->sc_opt);
                    }
                    unicode_buf[unicode_index] = '\0';
                    if (current_char == '}') {
                        long int unicode_value = strtol(unicode_buf, NULL, 16);
                        if (!scanner_buf_insert(&b_buffer,
                                                (char)unicode_value)) {
                            scanner_buf_free(&b_buffer);
                            parser_opt->return_code = INTER_ERR;
                            return false;
                        }
                        current_state = STRING;
                    } else {
                        // ERROR: invalid unicode sequence
                        scanner_buf_free(&b_buffer);
                        parser_opt->return_code = LEX_ERR;
                        return false;
                    }
                } else {
                    // ERROR: invalid unicode sequence
                    scanner_buf_free(&b_buffer);
                    parser_opt->return_code = LEX_ERR;
                    return false;
                }
                current_char = get_next_char(&parser_opt->sc_opt);
                break;

            case STRING_END:
                b_buffer.buffer[b_buffer.i] = '\0';
                token.type = TOKEN_STRING;
                token.line_index = parser_opt->sc_opt.line_counter;
                token.value.string = malloc(sizeof(char) * (b_buffer.i + 1));
                if (token.value.string == NULL) {
                    scanner_buf_free(&b_buffer);
                    parser_opt->return_code = INTER_ERR;
                    return false;
                }
                token.eol_before = new_line_before_token;
                token.id = parser_opt->sc_opt.id_counter++;
                strcpy(token.value.string, b_buffer.buffer);
                scanner_buf_free(&b_buffer);
                parser_opt->sc_opt.i--;
                parser_opt->token = token;
                return true;

            case QUESTION_MARK:
                next_char = get_next_char(&parser_opt->sc_opt);
                if (next_char == '?') {
                    current_state = NIL_COALESCING;
                } else {
                    // ERROR: invalid character after ?
                    parser_opt->return_code = LEX_ERR;
                    return false;
                }
                break;

            case OR:
                next_char = get_next_char(&parser_opt->sc_opt);
                if (next_char == '|') {
                    token.type = TOKEN_OR;
                    token.line_index = parser_opt->sc_opt.line_counter;
                    token.eol_before = new_line_before_token;
                    token.id = parser_opt->sc_opt.id_counter++;
                    parser_opt->token = token;
                    return true;
                } else {
                    // ERROR: invalid character after |
                    parser_opt->return_code = LEX_ERR;
                    return false;
                }
                break;

            case AND:
                next_char = get_next_char(&parser_opt->sc_opt);
                if (next_char == '&') {
                    token.type = TOKEN_AND;
                    token.line_index = parser_opt->sc_opt.line_counter;
                    token.eol_before = new_line_before_token;
                    token.id = parser_opt->sc_opt.id_counter++;
                    parser_opt->token = token;
                    return true;
                } else {
                    // ERROR: invalid character after &
                    parser_opt->return_code = LEX_ERR;
                    return false;
                }
                break;

            case NIL_COALESCING:
                token.type = TOKEN_NIL_COALESCING;
                token.line_index = parser_opt->sc_opt.line_counter;
                token.eol_before = new_line_before_token;
                token.id = parser_opt->sc_opt.id_counter++;
                parser_opt->token = token;
                return true;

            case WHITE_SPACE:
                new_line_before_token =
                    _skip_whitespaces(&parser_opt->sc_opt) ||
                    new_line_before_token;
                current_char = get_next_char(&parser_opt->sc_opt);
                current_state = START;
                break;

            case END_OF_FILE:
                token.type = TOKEN_END_OF_FILE;
                token.line_index = parser_opt->sc_opt.line_counter;
                token.id = parser_opt->sc_opt.id_counter++;
                parser_opt->token = token;
                return true;
        }
    }
}
