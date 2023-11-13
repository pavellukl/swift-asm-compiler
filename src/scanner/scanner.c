#include "scanner.h"

#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

TokenData is_keyword(char* keyword, bool new_line_before_token) {
    if (strcmp(keyword, "Double") == 0) {
        return (TokenData){.type = TOKEN_KEYWORD_DOUBLE,
                           .eol_before = new_line_before_token};
    }
    if (strcmp(keyword, "Double?") == 0) {
        return (TokenData){.type = TOKEN_KEYWORD_DOUBLE_NIL,
                           .eol_before = new_line_before_token};
    }
    if (strcmp(keyword, "else") == 0) {
        return (TokenData){.type = TOKEN_KEYWORD_ELSE,
                           .eol_before = new_line_before_token};
    }
    if (strcmp(keyword, "func") == 0) {
        return (TokenData){.type = TOKEN_KEYWORD_FUNC,
                           .eol_before = new_line_before_token};
    }
    if (strcmp(keyword, "if") == 0) {
        return (TokenData){.type = TOKEN_KEYWORD_IF,
                           .eol_before = new_line_before_token};
    }
    if (strcmp(keyword, "Int") == 0) {
        return (TokenData){.type = TOKEN_KEYWORD_INT,
                           .eol_before = new_line_before_token};
    }
    if (strcmp(keyword, "Int?") == 0) {
        return (TokenData){.type = TOKEN_KEYWORD_INT_NIL,
                           .eol_before = new_line_before_token};
    }
    if (strcmp(keyword, "let") == 0) {
        return (TokenData){.type = TOKEN_KEYWORD_LET,
                           .eol_before = new_line_before_token};
    }
    if (strcmp(keyword, "nil") == 0) {
        return (TokenData){.type = TOKEN_KEYWORD_NIL,
                           .eol_before = new_line_before_token};
    }
    if (strcmp(keyword, "return") == 0) {
        return (TokenData){.type = TOKEN_KEYWORD_RETURN,
                           .eol_before = new_line_before_token};
    }
    if (strcmp(keyword, "String") == 0) {
        return (TokenData){.type = TOKEN_KEYWORD_STRING,
                           .eol_before = new_line_before_token};
    }
    if (strcmp(keyword, "String?") == 0) {
        return (TokenData){.type = TOKEN_KEYWORD_STRING_NIL,
                           .eol_before = new_line_before_token};
    }
    if (strcmp(keyword, "var") == 0) {
        return (TokenData){.type = TOKEN_KEYWORD_VAR,
                           .eol_before = new_line_before_token};
    }
    if (strcmp(keyword, "while") == 0) {
        return (TokenData){.type = TOKEN_KEYWORD_WHILE,
                           .eol_before = new_line_before_token};
    }
    if (strcmp(keyword, "true") == 0) {
        return (TokenData){.type = TOKEN_KEYWORD_TRUE,
                           .value.boolean = true,
                           .eol_before = new_line_before_token};
    }
    if (strcmp(keyword, "false") == 0) {
        return (TokenData){.type = TOKEN_KEYWORD_FALSE,
                           .value.boolean = false,
                           .eol_before = new_line_before_token};
    }
    if (strcmp(keyword, "Bool") == 0) {
        return (TokenData){.type = TOKEN_KEYWORD_BOOL,
                           .value.boolean = true,
                           .eol_before = new_line_before_token};
    }
    if (strcmp(keyword, "Bool?") == 0) {
        return (TokenData){.type = TOKEN_KEYWORD_BOOL_NIL,
                           .value.boolean = false,
                           .eol_before = new_line_before_token};
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
    opt->line_counter = 1;

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
}

void scanner_rewind_file(ScannerOptions* opt) {
    opt->i = 0;
    opt->line_counter = 0;
}

int get_next_char(ScannerOptions* opt) {
    PRINTF_STDDEBUG("INDEX: %d   ", opt->i);
    if (opt->i < opt->size) {
        PRINTF_STDDEBUG("RETURNING CHAR: %c(%d)\n", opt->file[opt->i],
                        opt->file[opt->i]);

        return opt->file[opt->i++];
    } else {
        PRINTF_STDDEBUG("RETURNING CHAR: EOF\n");

        return EOF;
    }
}

TokenData get_next_token(ScannerOptions* opt) {
    PRINTF_STDDEBUG("CALL\n");
    TokenData token;
    AutomatState current_state = START;

    char b_buffer[256];  // adjust the size
    int buffer_index = 0;

    char unicode_buf[9];  // Holds up to 8 hexadecimal digits + null terminator.
    int unicode_index = 0;

    char current_char = get_next_char(opt);
    char next_char;

    int block_comment_count = 0;

    bool new_line_before_token = false;

    while (true) {
        PRINTF_STDDEBUG("STATE: %d, working with CHAR: %c(%d)\n ",
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
                        opt->line_counter++;
                        current_char = get_next_char(opt);
                        current_state = START;
                        break;

                    case EOF:
                        current_state = END_OF_FILE;
                        break;

                    case '"':
                        current_char = get_next_char(opt);
                        current_state = STRING;
                        break;

                    default:
                        if (isdigit(current_char)) {
                            current_state = NUM;
                        } else if (isalpha(current_char)) {
                            current_state = IDENTIF;
                        } else {
                            printf("LEX ERROR\n");
                        }
                        break;
                }
                break;

            case DIV:
                next_char = get_next_char(opt);
                if (next_char == '/') {
                    current_state = LINE_COMMENT;
                } else if (next_char == '*') {
                    current_state = BLOCK_COMMENT;
                    block_comment_count++;
                } else {
                    token.type = TOKEN_DIV;
                    token.line_index = opt->line_counter;
                    token.eol_before = new_line_before_token;
                    opt->i--;
                    return token;
                }
                break;

            case LINE_COMMENT:
                while (((next_char = get_next_char(opt)) != '\n') &&
                       (next_char != EOF)) {
                    // skipping characters
                }
                if (next_char == EOF) {
                    opt->i--;
                }
                token.type = TOKEN_LINE_COMMENT;
                token.line_index = opt->line_counter;
                token.eol_before = new_line_before_token;
                return token;

            case BLOCK_COMMENT:
                next_char = get_next_char(opt);
                while (next_char != EOF) {
                    if (next_char == '*') {
                        current_state = BLOCK_COMMENT_END;
                    } else if (next_char == '/') {
                        char next_char_peek = get_next_char(opt);
                        if (next_char_peek == '*') {
                            block_comment_count++;
                        } else {
                            opt->i--;
                        }
                    }
                    if (current_state == BLOCK_COMMENT) {
                        next_char = get_next_char(opt);
                    } else {
                        break;
                    }
                }
                if (next_char == EOF && block_comment_count > 0) {
                    // LEX ERROR comment not closed before eof
                }
                break;

            case BLOCK_COMMENT_END:
                next_char = get_next_char(opt);
                if (next_char == '/') {
                    block_comment_count--;
                    if (block_comment_count == 0) {
                        token.type = TOKEN_BLOCK_COMMENT;
                        token.line_index = opt->line_counter;
                        token.eol_before = new_line_before_token;
                        return token;
                    } else {
                        current_state = BLOCK_COMMENT;
                        next_char = get_next_char(opt);
                    }
                } else {
                    if (next_char != '*') {
                        current_state = BLOCK_COMMENT;
                    }
                }
                break;

            case ADD:
                token.type = TOKEN_ADD;
                token.line_index = opt->line_counter;
                token.eol_before = new_line_before_token;
                return token;

            case SUB:
                next_char = get_next_char(opt);
                if (next_char == '>') {
                    current_state = ARROW;
                } else {
                    token.type = TOKEN_SUB;
                    token.line_index = opt->line_counter;
                    token.eol_before = new_line_before_token;
                    opt->i--;
                    return token;
                }
                break;

            case ARROW:
                token.type = TOKEN_ARROW;
                token.line_index = opt->line_counter;
                token.eol_before = new_line_before_token;
                return token;

            case MUL:
                token.type = TOKEN_MUL;
                token.line_index = opt->line_counter;
                token.eol_before = new_line_before_token;
                return token;

            case EXCL_MARK:
                next_char = get_next_char(opt);
                if (next_char == '=') {
                    current_state = NOT_EQUAL;
                } else if (isalpha(next_char) || next_char == '_') {
                    token.type = TOKEN_NOT;
                    token.line_index = opt->line_counter;
                    token.eol_before = new_line_before_token;
                    opt->i--;
                    return token;
                } else {
                    token.type = TOKEN_EXCL_MARK;
                    token.line_index = opt->line_counter;
                    token.eol_before = new_line_before_token;
                    opt->i--;
                    return token;
                }
                break;

            case NOT_EQUAL:
                token.type = TOKEN_NOT_EQUAL;
                token.line_index = opt->line_counter;
                token.eol_before = new_line_before_token;
                return token;

            case ASSIGN:
                next_char = get_next_char(opt);
                if (next_char == '=') {
                    current_state = EQUAL;
                } else {
                    token.type = TOKEN_ASSIGN;
                    token.line_index = opt->line_counter;
                    token.eol_before = new_line_before_token;
                    opt->i--;
                    return token;
                }
                break;

            case EQUAL:
                token.type = TOKEN_EQUAL;
                token.line_index = opt->line_counter;
                token.eol_before = new_line_before_token;
                return token;

            case GREATER:
                next_char = get_next_char(opt);
                if (next_char == '=') {
                    current_state = GREATER_EQUAL;
                } else {
                    token.type = TOKEN_GREATER;
                    token.line_index = opt->line_counter;
                    token.eol_before = new_line_before_token;
                    opt->i--;
                    return token;
                }
                break;

            case LESSER:
                next_char = get_next_char(opt);
                if (next_char == '=') {
                    current_state = LESSER_EQUAL;
                } else {
                    token.type = TOKEN_LESSER;
                    token.line_index = opt->line_counter;
                    token.eol_before = new_line_before_token;
                    opt->i--;
                    return token;
                }
                break;

            case GREATER_EQUAL:
                token.type = TOKEN_GREATER_EQUAL;
                token.line_index = opt->line_counter;
                token.eol_before = new_line_before_token;
                return token;

            case LESSER_EQUAL:
                token.type = TOKEN_LESSER_EQUAL;
                token.line_index = opt->line_counter;
                token.eol_before = new_line_before_token;
                return token;

            case L_BRACKET:
                token.type = TOKEN_L_BRACKET;
                token.line_index = opt->line_counter;
                token.eol_before = new_line_before_token;
                return token;

            case R_BRACKET:
                token.type = TOKEN_R_BRACKET;
                token.line_index = opt->line_counter;
                token.eol_before = new_line_before_token;
                return token;

            case L_CRLY_BRACKET:
                token.type = TOKEN_L_CRLY_BRACKET;
                token.line_index = opt->line_counter;
                token.eol_before = new_line_before_token;
                return token;

            case R_CRLY_BRACKET:
                token.type = TOKEN_R_CRLY_BRACKET;
                token.line_index = opt->line_counter;
                token.eol_before = new_line_before_token;
                return token;

            case COLON:
                token.type = TOKEN_COLON;
                token.line_index = opt->line_counter;
                token.eol_before = new_line_before_token;
                return token;

            case COMA:
                token.type = TOKEN_COMA;
                token.line_index = opt->line_counter;
                token.eol_before = new_line_before_token;
                return token;

            case UNDERSCORE:
                next_char = get_next_char(opt);
                if (next_char == ' ' || next_char == ':') {
                    token.type = TOKEN_UNDERSCORE;
                    token.line_index = opt->line_counter;
                    token.eol_before = new_line_before_token;
                    return token;
                } else {
                    opt->i--;
                    current_state = IDENTIF;
                }
                break;

            case IDENTIF:
                while (isalpha(current_char) || current_char == '_' ||
                       isdigit(current_char)) {
                    b_buffer[buffer_index++] = current_char;
                    current_char = get_next_char(opt);
                }
                b_buffer[buffer_index] = '\0';

                if (current_char == '?') {
                    b_buffer[buffer_index++] = current_char;
                    b_buffer[buffer_index] = '\0';
                    token = is_keyword(b_buffer, new_line_before_token);
                    if (token.type != TOKEN_IDENTIF) {
                        token.line_index = opt->line_counter;
                        token.eol_before = new_line_before_token;
                        return token;
                    } else {
                        // ERROR: identifier (not keyword) with '?' suffix
                    }
                }
                //! opt->i--;
                token = is_keyword(b_buffer, new_line_before_token);
                if (token.type != TOKEN_IDENTIF) {
                    token.line_index = opt->line_counter;
                    token.eol_before = new_line_before_token;
                    return token;
                } else {
                    token.value.string =
                        (char*)malloc(sizeof(char) * (strlen(b_buffer) + 1));
                    strcpy(token.value.string, b_buffer);
                    token.line_index = opt->line_counter;
                    token.eol_before = new_line_before_token;
                    return token;
                }
                break;

            case NUM:  // Whole part
                if (isdigit(current_char)) {
                    b_buffer[buffer_index++] = current_char;
                    current_char = get_next_char(opt);
                } else if (current_char == '.') {
                    b_buffer[buffer_index++] = current_char;
                    current_state = DECIMAL_POINT;
                    current_char = get_next_char(opt);
                } else if (current_char == 'e' || current_char == 'E') {
                    b_buffer[buffer_index++] = current_char;
                    current_state = EXP_MARK;
                    current_char = get_next_char(opt);
                } else {
                    b_buffer[buffer_index] = '\0';
                    // ! opt->i--;
                    token.type = TOKEN_NUMBER;
                    token.num_type = INT;
                    token.line_index = opt->line_counter;
                    token.value.int_value = strtol(b_buffer, NULL, 10);
                    token.eol_before = new_line_before_token;
                    return token;
                }
                break;

            case DECIMAL_POINT:  // Decimal point
                if (isdigit(current_char)) {
                    b_buffer[buffer_index++] = current_char;
                    current_state = DECIMAL_VALUE;
                    current_char = get_next_char(opt);
                } else {
                    // Error: Decimal point not followed by digits
                }
                break;

            case DECIMAL_VALUE:  // Decimal part
                if (isdigit(current_char)) {
                    b_buffer[buffer_index++] = current_char;
                    current_char = get_next_char(opt);
                } else if (current_char == 'e' || current_char == 'E') {
                    b_buffer[buffer_index++] = current_char;
                    current_state = EXP_MARK;
                    current_char = get_next_char(opt);
                } else {
                    b_buffer[buffer_index] = '\0';
                    opt->i--;
                    token.type = TOKEN_NUMBER;
                    token.num_type = FLOAT;
                    token.line_index = opt->line_counter;
                    token.value.float_value = strtod(b_buffer, NULL);
                    token.eol_before = new_line_before_token;
                    return token;
                }
                break;

            case EXP_MARK:  // Exponent marker e/E
                if (current_char == '+' || current_char == '-') {
                    b_buffer[buffer_index++] = current_char;
                    current_state = EXP_SIGN;
                    current_char = get_next_char(opt);
                } else if (isdigit(current_char)) {
                    b_buffer[buffer_index++] = current_char;
                    current_state = EXPONENT;
                    current_char = get_next_char(opt);
                } else {
                    // Error: Exponent marker not followed by sign or digits
                }
                break;

            case EXP_SIGN:  // Sign after exponent marker +/-
                if (isdigit(current_char)) {
                    b_buffer[buffer_index++] = current_char;
                    current_state = EXPONENT;
                    current_char = get_next_char(opt);
                } else {
                    // Error: Exponent sign not followed by digits
                }
                break;

            case EXPONENT:  // Exponent digits
                if (isdigit(current_char)) {
                    b_buffer[buffer_index++] = current_char;
                    current_char = get_next_char(opt);
                } else {
                    b_buffer[buffer_index] = '\0';
                    opt->i--;
                    token.type = TOKEN_NUMBER;
                    token.num_type = FLOAT;
                    token.line_index = opt->line_counter;
                    token.value.float_value = strtod(b_buffer, NULL);
                    token.eol_before = new_line_before_token;
                    return token;
                }
                break;

            case STRING:
                if (current_char == '"') {
                    current_state = STRING_END;
                } else if (current_char == '\\') {
                    b_buffer[buffer_index++] = current_char;
                    current_state = ESCAPE_SEQUENCE;
                } else {
                    b_buffer[buffer_index++] = current_char;
                }
                current_char = get_next_char(opt);
                break;

            case ESCAPE_SEQUENCE:
                if (current_char == '\\' || current_char == '"' ||
                    current_char == 'n' || current_char == 'r' ||
                    current_char == 't') {
                    b_buffer[buffer_index++] = current_char;
                    current_state = STRING;
                } else if (current_char == 'u') {
                    b_buffer[buffer_index++] = current_char;
                    current_state = UNICODE_SEQUENCE;
                } else {
                    // ERROR: invalid escape sequence
                }
                current_char = get_next_char(opt);
                break;

            case UNICODE_SEQUENCE:
                if (current_char == '{') {
                    current_state = UNICODE_VALUE;
                } else {
                    // ERROR: invalid unicode sequence
                }
                current_char = get_next_char(opt);
                break;

            case UNICODE_VALUE:
                while (isxdigit(current_char) && unicode_index < 8) {
                    unicode_buf[unicode_index++] = current_char;
                    current_char = get_next_char(opt);
                }
                unicode_buf[unicode_index] = '\0';
                if (current_char == '}') {
                    // to do
                    strcat(b_buffer, unicode_buf);
                    buffer_index += unicode_index;
                    current_state = STRING;
                } else {
                    // ERROR: invalid unicode sequence
                }
                current_char = get_next_char(opt);
                break;

            case STRING_END:
                b_buffer[buffer_index] = '\0';
                token.type = TOKEN_STRING;
                token.line_index = opt->line_counter;
                token.value.string =
                    (char*)malloc(sizeof(char) * (strlen(b_buffer) + 1));
                token.eol_before = new_line_before_token;
                strcpy(token.value.string, b_buffer);
                //! opt->i--;
                return token;

            case QUESTION_MARK:
                next_char = get_next_char(opt);
                if (next_char == '?') {
                    current_state = NIL_COALESCING;
                } else {
                    // ERROR: invalid character after ?
                }
                break;

            case OR:
                next_char = get_next_char(opt);
                if (next_char == '|') {
                    token.type = TOKEN_OR;
                    token.line_index = opt->line_counter;
                    token.eol_before = new_line_before_token;
                    return token;
                } else {
                    // ERROR: invalid character after |
                }
                break;

            case AND:
                next_char = get_next_char(opt);
                if (next_char == '&') {
                    token.type = TOKEN_AND;
                    token.line_index = opt->line_counter;
                    token.eol_before = new_line_before_token;
                    return token;
                } else {
                    // ERROR: invalid character after &
                }
                break;

            case NIL_COALESCING:
                token.type = TOKEN_NIL_COALESCING;
                token.line_index = opt->line_counter;
                token.eol_before = new_line_before_token;
                return token;

            case WHITE_SPACE:
                new_line_before_token = _skip_whitespaces(opt);
                current_char = get_next_char(opt);
                current_state = START;
                break;

            case END_OF_FILE:
                token.type = TOKEN_END_OF_FILE;
                token.line_index = opt->line_counter;
                return token;
        }
    }
}
