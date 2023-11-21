#include "utils.h"

bool clone_string(char **dest, char *src) {
    *dest = malloc(sizeof(char) * (strlen(src) + 1));
    if (*dest == NULL) {
        return false;
    }
    strcpy(*dest, src);
    return true;
}
