#include "../src/parser/precedence_parser/ListPP/ListPP.h"

#include <criterion/criterion.h>
#include <stdio.h>

#define INIT_TEST()                                                            \
    ListPP list;                                                               \
    list_pp_init(&list);                                                       \
    PPListItem item0 = {.data_type = T_INT};    /*0*/                          \
    PPListItem item1 = {.data_type = T_FLOAT};  /*1*/                          \
    PPListItem item2 = {.data_type = T_STRING}; /*2*/                          \
    PPListItem item3 = {.data_type = T_BOOL};   /*3*/

#define CLEAR_TEST()                                                           \
    list_pp_dispose(&list);

Test(list, insert_first) {
    INIT_TEST()

    list_pp_insert_first(&list, item0);
    list_pp_insert_first(&list, item1);
    list_pp_insert_first(&list, item2);
    list_pp_insert_first(&list, item3);

    cr_assert(list.activeItem == NULL);
    cr_assert(list.firstItem != NULL);
    cr_assert(list.firstItem->data.data_type == T_BOOL);
    cr_assert(list.firstItem->nextItem->nextItem->data.data_type == T_FLOAT);

    CLEAR_TEST()
}

Test(list, insert_after) {
    INIT_TEST()
    PPListItem item_data;

    // set list = [1, 3, 2, 0]
    list_pp_insert_first(&list, item0);
    list_pp_insert_first(&list, item1);
    list_pp_first(&list);
    list_pp_insert_after(&list, item2);
    list_pp_insert_after(&list, item3);

    cr_assert(list.firstItem != NULL);
    cr_assert(list_pp_get_value(&list, &item_data));
    cr_assert(item_data.data_type == T_FLOAT);

    list_pp_next(&list);
    cr_assert(list_pp_get_value(&list, &item_data));
    cr_assert(item_data.data_type == T_BOOL);

    CLEAR_TEST()
}

Test(list, insert_before) {
    INIT_TEST()
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
    cr_assert_eq(item_data.data_type, T_INT);

    list_pp_first(&list);
    list_pp_next(&list);
    list_pp_next(&list);
    cr_assert(list_pp_get_value(&list, &item_data));
    cr_assert(item_data.data_type == T_BOOL);

    CLEAR_TEST()
}

Test(list, delete_first) {
    INIT_TEST()

    list_pp_insert_first(&list, item0);
    list_pp_insert_first(&list, item1);
    list_pp_insert_first(&list, item2);
    list_pp_insert_first(&list, item3);

    list_pp_delete_first(&list);
    cr_assert(list.firstItem->data.data_type == T_STRING);

    list_pp_delete_first(&list);
    cr_assert(list.firstItem->data.data_type == T_FLOAT);

    CLEAR_TEST()
}

Test(list, delete_after) {
    INIT_TEST()

    list_pp_insert_first(&list, item0);
    list_pp_insert_first(&list, item1);
    list_pp_insert_first(&list, item2);
    list_pp_insert_first(&list, item3);

    list_pp_first(&list);
    list_pp_delete_after(&list);
    cr_assert(list.firstItem->nextItem->data.data_type == T_FLOAT);

    list_pp_delete_after(&list);
    cr_assert(list.firstItem->nextItem->data.data_type == T_INT);

    CLEAR_TEST()
}