#include "../src/helpers/string_buffer/string_buffer.h"

#include <criterion/criterion.h>
#include <criterion/new/assert.h>
#include <stdio.h>
#include <string.h>

Test(sbuffer, printing) {
    SBuffer *sbuffer = sbuffer_init();
    if (sbuffer == NULL) {
        cr_fail("Allocation error.");
    }

    char mes0[336] = "Message number 0, Message number 0, Message number 0, Message number 0, Message number 0, Message number 0, Message number 0Message number 0, Message number 0, Message number 0,Message number 0, Message number 0, Message number 0,Message number 0, Message number 0, Message number 0,Message number 0, Message number 0, Message number 0";
    sbuffer_printf(sbuffer, "%s", mes0);

    cr_assert(strlen(sbuffer->string) == 335);
    cr_assert(strcmp(mes0, sbuffer->string) == 0);

    char mes1[336] = "Message number 1, Message number 1, Message number 1, Message number 1, Message number 1, Message number 1, Message number 1Message number 1, Message number 1, Message number 1,Message number 1, Message number 1, Message number 1,Message number 1, Message number 1, Message number 1,Message number 1, Message number 1, Message number 1";
    sbuffer_printf(sbuffer, "%s", mes1);

    char mes0mes1[700];
    mes0mes1[0] = '\0';
    strcat(mes0mes1, mes0);
    strcat(mes0mes1, mes1);

    cr_assert(strcmp(mes0mes1, sbuffer->string) == 0);

    sbuffer_discard(sbuffer);
}
