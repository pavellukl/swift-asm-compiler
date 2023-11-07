#include "../src/helpers/list/list.h"

#include <criterion/criterion.h>
#include <stdio.h>

#define INIT_TEST()                                                            \
    ListPP list;                                                               \
    list_pp_init(&list);                                                       \
    PPListItem item0 = {.value.int_ = 0};                                      \
    PPListItem item1 = {.value.int_ = 1};                                      \
    PPListItem item2 = {.value.int_ = 2};                                      \
    PPListItem item3 = {.value.int_ = 3};

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
    cr_assert(list.firstItem->data.value.int_ == 3);
    cr_assert(list.firstItem->nextItem->nextItem->data.value.int_ == 1);

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
    cr_assert(item_data.value.int_ == 1);

    list_pp_next(&list);
    cr_assert(list_pp_get_value(&list, &item_data));
    cr_assert(item_data.value.int_ == 3);

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
    cr_assert_eq(item_data.value.int_, 0);

    list_pp_first(&list);
    list_pp_next(&list);
    list_pp_next(&list);
    cr_assert(list_pp_get_value(&list, &item_data));
    cr_assert(item_data.value.int_ == 3);

    CLEAR_TEST()
}

Test(list, delete_first) {
    INIT_TEST()

    list_pp_insert_first(&list, item0);
    list_pp_insert_first(&list, item1);
    list_pp_insert_first(&list, item2);
    list_pp_insert_first(&list, item3);

    list_pp_delete_first(&list);
    cr_assert(list.firstItem->data.value.int_ == 2);

    list_pp_delete_first(&list);
    cr_assert(list.firstItem->data.value.int_ == 1);

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
    cr_assert(list.firstItem->nextItem->data.value.int_ == 1);

    list_pp_delete_after(&list);
    cr_assert(list.firstItem->nextItem->data.value.int_ == 0);

    CLEAR_TEST()
}