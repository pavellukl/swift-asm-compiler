#include "../src/parser/precedence_parser/ListPP/ListPP.h"

#include <criterion/criterion.h>
#include <stdio.h>

#define INITERMINAL_TEST()                                                     \
    ListPP list;                                                               \
    list_pp_init(&list);                                                       \
    PPListItem item0 = {.pp_type = TERMINAL_INT};    /*0*/                     \
    PPListItem item1 = {.pp_type = TERMINAL_FLOAT};  /*1*/                     \
    PPListItem item2 = {.pp_type = TERMINAL_STRING}; /*2*/                     \
    PPListItem item3 = {.pp_type = TERMINAL_BOOL};   /*3*/

#define CLEAR_TEST()                                                           \
    list_pp_dispose(&list);

Test(list, insert_first) {
    INITERMINAL_TEST()

    list_pp_insert_first(&list, item0);
    list_pp_insert_first(&list, item1);
    list_pp_insert_first(&list, item2);
    list_pp_insert_first(&list, item3);

    cr_assert(list.activeItem == NULL);
    cr_assert(list.firstItem != NULL);
    cr_assert(list.firstItem->data.pp_type == TERMINAL_BOOL);
    cr_assert(list.firstItem->nextItem->nextItem->data.pp_type == TERMINAL_FLOAT);

    CLEAR_TEST()
}

Test(list, insert_after) {
    INITERMINAL_TEST()
    PPListItem item_data;

    // set list = [1, 3, 2, 0]
    list_pp_insert_first(&list, item0);
    list_pp_insert_first(&list, item1);
    list_pp_first(&list);
    list_pp_insert_after(&list, item2);
    list_pp_insert_after(&list, item3);

    cr_assert(list.firstItem != NULL);
    cr_assert(list_pp_get_value(&list, &item_data));
    cr_assert(item_data.pp_type == TERMINAL_FLOAT);

    list_pp_next(&list);
    cr_assert(list_pp_get_value(&list, &item_data));
    cr_assert(item_data.pp_type == TERMINAL_BOOL);

    CLEAR_TEST()
}

Test(list, insert_before) {
    INITERMINAL_TEST()
    PPListItem item_data;

    // set list = [1, 2, 3, 0]
    list_pp_insert_first(&list, item0);
    list_pp_insert_first(&list, item1);
    list_pp_first(&list);
    list_pp_next(&list);
    list_pp_insert_before(&list, item2);
    list_pp_insert_before(&list, item3);

    cr_assert(list.firstItem != NULL);
    cr_assert(list_pp_get_value(&list, &item_data));
    cr_assert_eq(item_data.pp_type, TERMINAL_INT);

    list_pp_first(&list);
    list_pp_next(&list);
    list_pp_next(&list);
    cr_assert(list_pp_get_value(&list, &item_data));
    cr_assert(item_data.pp_type == TERMINAL_BOOL);

    CLEAR_TEST()
}

Test(list, delete_first) {
    INITERMINAL_TEST()

    list_pp_insert_first(&list, item0);
    list_pp_insert_first(&list, item1);
    list_pp_insert_first(&list, item2);
    list_pp_insert_first(&list, item3);

    list_pp_delete_first(&list);
    cr_assert(list.firstItem->data.pp_type == TERMINAL_STRING);

    list_pp_delete_first(&list);
    cr_assert(list.firstItem->data.pp_type == TERMINAL_FLOAT);

    CLEAR_TEST()
}

Test(list, delete_after) {
    INITERMINAL_TEST()

    list_pp_insert_first(&list, item0);
    list_pp_insert_first(&list, item1);
    list_pp_insert_first(&list, item2);
    list_pp_insert_first(&list, item3);

    list_pp_first(&list);
    list_pp_delete_after(&list);
    cr_assert(list.firstItem->nextItem->data.pp_type == TERMINAL_FLOAT);

    list_pp_delete_after(&list);
    cr_assert(list.firstItem->nextItem->data.pp_type == TERMINAL_INT);

    CLEAR_TEST()
}