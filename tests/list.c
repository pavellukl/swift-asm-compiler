#include "../src/helpers/list/list.h"

#include <criterion/criterion.h>
#include <stdio.h>

#define INIT_TEST()                                                            \
    ListPPR list;                                                              \
    list_ppr_init(&list);                                                      \
    PPRListItem item0 = {.value.int_ = 0};                                     \
    PPRListItem item1 = {.value.int_ = 1};                                     \
    PPRListItem item2 = {.value.int_ = 2};                                     \
    PPRListItem item3 = {.value.int_ = 3};

#define CLEAR_TEST()                                                           \
    list_ppr_dispose(&list);

Test(list, insert_first) {
    INIT_TEST()

    list_ppr_insert_first(&list, item0);
    list_ppr_insert_first(&list, item1);
    list_ppr_insert_first(&list, item2);
    list_ppr_insert_first(&list, item3);

    cr_assert(list.activeItem == NULL);
    cr_assert(list.firstItem != NULL);
    cr_assert(list.firstItem->data.value.int_ == 3);
    cr_assert(list.firstItem->nextItem->nextItem->data.value.int_ == 1);

    CLEAR_TEST()
}

Test(list, insert_after) {
    INIT_TEST()
    PPRListItem item_data;

    // set list = [1, 3, 2, 0]
    list_ppr_insert_first(&list, item0);
    list_ppr_insert_first(&list, item1);
    list_ppr_first(&list);
    list_ppr_insert_after(&list, item2);
    list_ppr_insert_after(&list, item3);

    cr_assert(list.firstItem != NULL);
    cr_assert(list_ppr_get_value(&list, &item_data));
    cr_assert(item_data.value.int_ == 1);

    list_ppr_next(&list);
    cr_assert(list_ppr_get_value(&list, &item_data));
    cr_assert(item_data.value.int_ == 3);

    CLEAR_TEST()
}

Test(list, insert_before) {
    INIT_TEST()
    PPRListItem item_data;

    // set list = [1, 2, 3, 0]
    list_ppr_insert_first(&list, item0);
    list_ppr_insert_first(&list, item1);
    list_ppr_first(&list);
    list_ppr_next(&list);
    list_ppr_insert_before(&list, item2);
    list_ppr_insert_before(&list, item3);

    cr_assert(list.firstItem != NULL);
    cr_assert(list_ppr_get_value(&list, &item_data));
    cr_assert_eq(item_data.value.int_, 0);

    list_ppr_first(&list);
    list_ppr_next(&list);
    list_ppr_next(&list);
    cr_assert(list_ppr_get_value(&list, &item_data));
    cr_assert(item_data.value.int_ == 3);

    CLEAR_TEST()
}

Test(list, delete_first) {
    INIT_TEST()

    list_ppr_insert_first(&list, item0);
    list_ppr_insert_first(&list, item1);
    list_ppr_insert_first(&list, item2);
    list_ppr_insert_first(&list, item3);

    list_ppr_delete_first(&list);
    cr_assert(list.firstItem->data.value.int_ == 2);

    list_ppr_delete_first(&list);
    cr_assert(list.firstItem->data.value.int_ == 1);

    CLEAR_TEST()
}

Test(list, delete_after) {
    INIT_TEST()

    list_ppr_insert_first(&list, item0);
    list_ppr_insert_first(&list, item1);
    list_ppr_insert_first(&list, item2);
    list_ppr_insert_first(&list, item3);

    list_ppr_first(&list);
    list_ppr_delete_after(&list);
    cr_assert(list.firstItem->nextItem->data.value.int_ == 1);

    list_ppr_delete_after(&list);
    cr_assert(list.firstItem->nextItem->data.value.int_ == 0);

    CLEAR_TEST()
}