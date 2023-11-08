#include "scanner.h"

TokenData is_keyword(char* keyword) {
    if (strcmp(keyword, "Double") == 0) {
        return (TokenData){.type = TOKEN_KEYWORD_DOUBLE};
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

    return (TokenData){.type = TOKEN_IDENTIF};
}

TokenData is_built_in_function(char* keyword) {
    if (strcmp(keyword, "readString") == 0) {
        return (TokenData){.type = TOKEN_FUNC_READ_STRING};
    }
    if (strcmp(keyword, "readInt") == 0) {
        return (TokenData){.type = TOKEN_FUNC_READ_INT};
    }
    if (strcmp(keyword, "readDouble") == 0) {
        return (TokenData){.type = TOKEN_FUNC_READ_DOUBLE};
    }
    if (strcmp(keyword, "write") == 0) {
        return (TokenData){.type = TOKEN_FUNC_WRITE};
    }
    if (strcmp(keyword, "Int2Double") == 0) {
        return (TokenData){.type = TOKEN_FUNC_INT_2_DOUBLE};
    }
    if (strcmp(keyword, "Double2Int") == 0) {
        return (TokenData){.type = TOKEN_FUNC_DOUBLE_2_INT};
    }
    if (strcmp(keyword, "length") == 0) {
        return (TokenData){.type = TOKEN_FUNC_LENGTH};
    }
    if (strcmp(keyword, "substring") == 0) {
        return (TokenData){.type = TOKEN_FUNC_SUBSTRING};
    }
    if (strcmp(keyword, "ord") == 0) {
        return (TokenData){.type = TOKEN_FUNC_ORD};
    }
    if (strcmp(keyword, "chr") == 0) {
        return (TokenData){.type = TOKEN_FUNC_CHR};
    }
    return (TokenData){.type = TOKEN_IDENTIF};
}

void _skip_whitespaces(FILE* input) {
    char ch;
    while ((ch = fgetc(input)) != EOF) {
        if ((!isspace(ch)) || (ch == '\n')) {
            ungetc(ch, input);
            break;
        }
    }
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

// TokenData get_next_token(ScannerOptions opt) {
TokenData get_next_token(FILE* input, int line_counter) {
    TokenData token;
    AutomatState current_state = START;

    char b_buffer[256];  // adjust the size
    int buffer_index = 0;

    char unicode_buf[9];  // Holds up to 8 hexadecimal digits + null terminator.
    int unicode_index = 0;

    char current_char = fgetc(input);
    char next_char;

    while (true) {
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

                    case ' ':
                    case '\0':  // aj null?
                    case '\t':
                    case '\v':
                    case '\f':
                    case '\r':
                        current_char = fgetc(input);
                        continue;

                    case '\n':
                        current_state = NEW_LINE;
                        break;

                    case EOF:
                        current_state = END_OF_FILE;
                        break;

                    case '"':
                        current_char = fgetc(input);
                        current_state = STRING;
                        break;

                    default:
                        if (isdigit(current_char)) {
                            current_state = NUM;
                        } else if (isalpha(current_char) || '_') {
                            current_state = IDENTIF;
                        } else {
                            printf("LEX ERROR\n", current_char);
                        }
                        break;
                }
                break;

            case DIV:
                next_char = fgetc(input);
                if (next_char == '/') {
                    current_state = LINE_COMMENT;
                } else if (next_char == '*') {
                    current_state = BLOCK_COMMENT;
                } else {
                    token.type = TOKEN_DIV;
                    token.line_index = line_counter;
                    ungetc(next_char, input);
                    return token;
                }
                break;

            case LINE_COMMENT:
                while (((next_char = fgetc(input)) != '\n') &&
                       (next_char != EOF)) {
                    // skipping characters
                }
                if (next_char == EOF) {
                    ungetc(next_char, input);
                }
                token.type = TOKEN_LINE_COMMENT;
                token.line_index = line_counter;
                return token;

            case BLOCK_COMMENT:
                next_char = fgetc(input);
                while (next_char != EOF) {
                    if (next_char == '*') {
                        current_state = BLOCK_COMMENT_END;
                        break;
                    }
                    next_char = fgetc(input);
                }
                break;

            case BLOCK_COMMENT_END:
                next_char = fgetc(input);
                while (next_char != EOF) {
                    if (next_char == '/') {
                        token.type = TOKEN_BLOCK_COMMENT;
                        token.line_index = line_counter;
                        return token;
                    } else if (next_char == '*') {
                        // Stay in the BLOCK_COMMENT_END state
                    } else {
                        current_state = BLOCK_COMMENT;
                        break;
                    }
                    next_char = fgetc(input);
                }
                break;

            case ADD:
                token.type = TOKEN_ADD;
                token.line_index = line_counter;
                return token;

            case SUB:
                next_char = fgetc(input);
                if (next_char == '>') {
                    current_state = ARROW;
                } else {
                    token.type = TOKEN_SUB;
                    token.line_index = line_counter;
                    ungetc(next_char, input);
                    return token;
                }
                break;

            case ARROW:
                token.type = TOKEN_ARROW;
                token.line_index = line_counter;
                return token;

            case MUL:
                token.type = TOKEN_MUL;
                token.line_index = line_counter;
                return token;

            case EXCL_MARK:
                next_char = fgetc(input);
                if (next_char == '=') {
                    current_state = NOT_EQUAL;
                } else {
                    token.type = TOKEN_EXCL_MARK;
                    token.line_index = line_counter;
                    ungetc(next_char, input);
                    return token;
                }
                break;

            case NOT_EQUAL:
                token.type = TOKEN_NOT_EQUAL;
                token.line_index = line_counter;
                return token;

            case ASSIGN:
                next_char = fgetc(input);
                if (next_char == '=') {
                    current_state = EQUAL;
                } else {
                    token.type = TOKEN_ASSIGN;
                    token.line_index = line_counter;
                    ungetc(next_char, input);
                    return token;
                }
                break;

            case EQUAL:
                token.type = TOKEN_EQUAL;
                token.line_index = line_counter;
                return token;

            case GREATER:
                next_char = fgetc(input);
                if (next_char == '=') {
                    current_state = GREATER_EQUAL;
                } else {
                    token.type = TOKEN_GREATER;
                    token.line_index = line_counter;
                    ungetc(next_char, input);
                    return token;
                }
                break;

            case LESSER:
                next_char = fgetc(input);
                if (next_char == '=') {
                    current_state = LESSER_EQUAL;
                } else {
                    token.type = TOKEN_LESSER;
                    token.line_index = line_counter;
                    ungetc(next_char, input);
                    return token;
                }
                break;

            case GREATER_EQUAL:
                token.type = TOKEN_GREATER_EQUAL;
                token.line_index = line_counter;
                return token;

            case LESSER_EQUAL:
                token.type = TOKEN_LESSER_EQUAL;
                token.line_index = line_counter;
                return token;

            case L_BRACKET:
                token.type = TOKEN_L_BRACKET;
                token.line_index = line_counter;
                return token;

            case R_BRACKET:
                token.type = TOKEN_R_BRACKET;
                token.line_index = line_counter;
                return token;

            case L_CRLY_BRACKET:
                token.type = TOKEN_L_CRLY_BRACKET;
                token.line_index = line_counter;
                return token;

            case R_CRLY_BRACKET:
                token.type = TOKEN_R_CRLY_BRACKET;
                token.line_index = line_counter;
                return token;

            case COLON:
                token.type = TOKEN_COLON;
                token.line_index = line_counter;
                return token;

            case COMA:
                token.type = TOKEN_COMA;
                token.line_index = line_counter;
                return token;

            case IDENTIF:
                while (isalpha(current_char) || current_char == '_' ||
                       isdigit(current_char)) {
                    b_buffer[buffer_index++] = current_char;
                    current_char = fgetc(input);
                }
                b_buffer[buffer_index] = '\0';

                if (current_char == '?') {
                    b_buffer[buffer_index++] = current_char;
                    b_buffer[buffer_index] = '\0';
                    token = is_keyword(b_buffer);
                    if (token.type != TOKEN_IDENTIF) {
                        token.line_index = line_counter;
                        return token;
                    } else {
                        // ERROR: identifier (not keyword) with '?' suffix
                    }
                }

                else if ((isspace(current_char)) && (current_char != '\n')) {
                    _skip_whitespaces(input);
                    current_char = fgetc(input);
                }

                if (current_char == '(') {
                    token = is_built_in_function(b_buffer);
                    if (token.type != TOKEN_IDENTIF) {
                        token.line_index = line_counter;
                        ungetc(current_char, input);
                        return token;
                    }
                }

                ungetc(current_char, input);
                token = is_keyword(b_buffer);
                if (token.type != TOKEN_IDENTIF) {
                    token.line_index = line_counter;
                    return token;
                } else {
                    token.value.string =
                        (char*)malloc(sizeof(char) * (strlen(b_buffer) + 1));
                    strcpy(token.value.string, b_buffer);
                    token.line_index = line_counter;
                    return token;
                }
                break;

            case NUM:  // Whole part
                if (isdigit(current_char)) {
                    b_buffer[buffer_index++] = current_char;
                    current_char = fgetc(input);
                } else if (current_char == '.') {
                    b_buffer[buffer_index++] = current_char;
                    current_state = DECIMAL_POINT;
                    current_char = fgetc(input);
                } else if (current_char == 'e' || current_char == 'E') {
                    b_buffer[buffer_index++] = current_char;
                    current_state = EXP_MARK;
                    current_char = fgetc(input);
                } else {
                    b_buffer[buffer_index] = '\0';
                    ungetc(current_char, input);
                    token.type = TOKEN_INT_NUM;
                    token.line_index = line_counter;
                    token.value.int_value = strtol(b_buffer, NULL, 10);
                    return token;
                }
                break;

            case DECIMAL_POINT:  // Decimal point
                if (isdigit(current_char)) {
                    b_buffer[buffer_index++] = current_char;
                    current_state = DECIMAL_VALUE;
                    current_char = fgetc(input);
                } else {
                    // Error: Decimal point not followed by digits
                }
                break;

            case DECIMAL_VALUE:  // Decimal part
                if (isdigit(current_char)) {
                    b_buffer[buffer_index++] = current_char;
                    current_char = fgetc(input);
                } else if (current_char == 'e' || current_char == 'E') {
                    b_buffer[buffer_index++] = current_char;
                    current_state = EXP_MARK;
                    current_char = fgetc(input);
                } else {
                    b_buffer[buffer_index] = '\0';
                    ungetc(current_char, input);
                    token.type = TOKEN_FLOAT_NUM;
                    token.line_index = line_counter;
                    token.value.float_value = strtod(b_buffer, NULL);
                    return token;
                }
                break;

            case EXP_MARK:  // Exponent marker e/E
                if (current_char == '+' || current_char == '-') {
                    b_buffer[buffer_index++] = current_char;
                    current_state = EXP_SIGN;
                    current_char = fgetc(input);
                } else if (isdigit(current_char)) {
                    b_buffer[buffer_index++] = current_char;
                    current_state = EXPONENT;
                    current_char = fgetc(input);
                } else {
                    // Error: Exponent marker not followed by sign or digits
                }
                break;

            case EXP_SIGN:  // Sign after exponent marker +/-
                if (isdigit(current_char)) {
                    b_buffer[buffer_index++] = current_char;
                    current_state = EXPONENT;
                    current_char = fgetc(input);
                } else {
                    // Error: Exponent sign not followed by digits
                }
                break;

            case EXPONENT:  // Exponent digits
                if (isdigit(current_char)) {
                    b_buffer[buffer_index++] = current_char;
                    current_char = fgetc(input);
                } else {
                    b_buffer[buffer_index] = '\0';
                    ungetc(current_char, input);
                    token.type = TOKEN_NUM_W_EXP;
                    token.line_index = line_counter;
                    token.value.float_value = strtod(b_buffer, NULL);
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
                current_char = fgetc(input);
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
                current_char = fgetc(input);
                break;

            case UNICODE_SEQUENCE:
                if (current_char == '{') {
                    current_state = UNICODE_VALUE;
                } else {
                    // ERROR: invalid unicode sequence
                }
                current_char = fgetc(input);
                break;

            case UNICODE_VALUE:
                while (isxdigit(current_char) && unicode_index < 8) {
                    unicode_buf[unicode_index++] = current_char;
                    current_char = fgetc(input);
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
                current_char = fgetc(input);
                break;

            case STRING_END:
                b_buffer[buffer_index] = '\0';
                token.type = TOKEN_STRING;
                token.line_index = line_counter;
                token.value.string =
                    (char*)malloc(sizeof(char) * (strlen(b_buffer) + 1));
                strcpy(token.value.string, b_buffer);
                ungetc(current_char, input);
                return token;

            case QUESTION_MARK:
                next_char = fgetc(input);
                if (next_char == '?') {
                    current_state = NIL_COALESCING;
                } else {
                    // ERROR: invalid character after ?
                }
                break;

            case NIL_COALESCING:
                token.type = TOKEN_NIL_COALESCING;
                token.line_index = line_counter;
                return token;

            case NEW_LINE:
                token.type = TOKEN_NEW_LINE;
                token.line_index = line_counter;
                _skip_whitespaces(input);
                return token;

            case END_OF_FILE:
                token.type = TOKEN_END_OF_FILE;
                token.line_index = line_counter;
                return token;
        }
    }
}
