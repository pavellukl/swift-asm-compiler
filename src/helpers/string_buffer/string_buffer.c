#include "string_buffer.h"

SBuffer *sbuffer_init() {
    SBuffer *sbuffer = malloc(sizeof (*sbuffer));
    if (sbuffer == NULL) {
        return NULL;
    }

    sbuffer->string = malloc(sizeof (*sbuffer->string) * SBUFFER_INIT_CAPACITY);
    if (sbuffer->string == NULL) {
        free(sbuffer);
        return NULL;
    }

    sbuffer->string[0] = '\0';
    sbuffer->capacity = SBUFFER_INIT_CAPACITY;
    sbuffer->size = 1;

    return sbuffer;
}

void sbuffer_discard(SBuffer *sbuffer) {
    free(sbuffer->string);
    free(sbuffer);
}

bool sbuffer_printf(SBuffer *sbuffer, const char *format, ...) {
    va_list args;

    va_start(args, format);
    size_t append_size = vsnprintf(NULL, 0, format, args);
    char *append = malloc(append_size + 1);
    if (append == NULL) {
        return false;
    }

    va_start(args, format);
    vsprintf(append, format, args);

    va_end(args);

    if (sbuffer->capacity < sbuffer->size + append_size) {
        while (sbuffer->capacity < sbuffer->size + append_size)
        {
            sbuffer->capacity *= 2;
        }
        
        sbuffer->string = realloc(sbuffer->string, sizeof(*sbuffer->string) * sbuffer->capacity);
        if (sbuffer->string == NULL) {
            sbuffer->capacity = 0;
            sbuffer->size = 0;
            free(append);
            return false;
        }
    }

    strcat(sbuffer->string, append);
    sbuffer->size += append_size;

    free(append);
    return true;
}

bool sbuffer_clear(SBuffer **sbuffer) {
    sbuffer_discard(*sbuffer);
    *sbuffer = sbuffer_init();
    if (*sbuffer == NULL) {
        return false;
    }
    return true;
}
