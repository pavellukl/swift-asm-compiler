/**
 * @name Implementation of IFJ23 language compiler
 * @file list.h
 * @brief Contains definitions of structs, functions and macros for the list.
 *
 * @author Jan Klanica (xklani00)
 */

#ifndef LIST_H
#define LIST_H

#include <stdbool.h>
#include <stdlib.h>

/// @brief Return value from some functions operating upon list.
typedef enum { LIST_OK, LIST_NOT_ACTIVE, LIST_ALLOC_ERR } ListReturnCode;

/*
 * Macro generating declarations of functions operating upon list.
 *
 * @param T Data type of items stored in the list.
 * @param TNAME_PASCAL Any name for chosen data type in pascal case.
 * @param TNAME_SNAKE Any name for chosen data type in snake case.
 */

#define LIST_DEC(T, TNAME_PASCAL, TNAME_SNAKE)                                 \
                                                                               \
    /** An item of the list. */                                                \
    typedef struct ListItem##TNAME_PASCAL {                                    \
        /** Stored value. */                                                   \
        T data;                                                                \
        /** Pointer to the next item of the list. */                           \
        struct ListItem##TNAME_PASCAL *nextItem;                               \
    } ListItem##TNAME_PASCAL;                                                  \
                                                                               \
    /** List representation. */                                                \
    typedef struct {                                                           \
        /** Pointer to the first item of the list. */                          \
        ListItem##TNAME_PASCAL *firstItem;                                     \
        /** Pointer to the active item of the list. */                         \
        ListItem##TNAME_PASCAL *activeItem;                                    \
    } List##TNAME_PASCAL;                                                      \
                                                                               \
    /**                                                                        \
     * @brief Initializes a new list.                                          \
     *                                                                         \
     * @param list Pointer to the list.                                        \
     */                                                                        \
    void list_##TNAME_SNAKE##_init(List##TNAME_PASCAL *list);                  \
                                                                               \
    /**                                                                        \
     * @brief Inserts @p data as the first item in the list.                   \
     *                                                                         \
     * @param list Pointer to the list to operate upon.                        \
     * @param data Value to be inserted in the stack.                          \
     * @return LIST_OK On Success.                                             \
     * @return LIST_ALLOC_ERR If allocation error was encountered.             \
     */                                                                        \
    ListReturnCode list_##TNAME_SNAKE##_insert_first(List##TNAME_PASCAL *list, \
                                                     T data);                  \
                                                                               \
    /**                                                                        \
     * @brief Inserts @p data after the active item in the list.               \
     *                                                                         \
     * @param list Pointer to the list to operate upon.                        \
     * @param data Value to be inserted in the stack.                          \
     * @return LIST_OK On Success.                                             \
     * @return LIST_NOT_ACTIVE If list is not active.                          \
     * @return LIST_ALLOC_ERR If allocation error was encountered.             \
     */                                                                        \
    ListReturnCode list_##TNAME_SNAKE##_insert_after(List##TNAME_PASCAL *list, \
                                                     T data);                  \
                                                                               \
    /**                                                                        \
     * @brief Inserts @p data before the active item in the list.              \
     *                                                                         \
     * @param list Pointer to the list to operate upon.                        \
     * @param data Value to be inserted in the stack.                          \
     * @return LIST_OK On Success.                                             \
     * @return LIST_NOT_ACTIVE If list is not active.                          \
     * @return LIST_ALLOC_ERR If allocation error was encountered.             \
     */                                                                        \
    ListReturnCode list_##TNAME_SNAKE##_insert_before(                         \
        List##TNAME_PASCAL *list, T data);                                     \
                                                                               \
    /**                                                                        \
     * @brief Removes the first item from the list. If the removing item is    \
     * active, no item is active afterwards.                                   \
     *                                                                         \
     * @param list Pointer to the list to operate upon.                        \
     */                                                                        \
    void list_##TNAME_SNAKE##_delete_first(List##TNAME_PASCAL *list);          \
                                                                               \
    /**                                                                        \
     * @brief Removes an item which comes after the active item in the list.   \
     *                                                                         \
     * @param list Pointer to the list to operate upon.                        \
     */                                                                        \
    void list_##TNAME_SNAKE##_delete_after(List##TNAME_PASCAL *list);          \
                                                                               \
    /**                                                                        \
     * @brief Sets the first item as the active item.                          \
     *                                                                         \
     * @param list Pointer to the list to operate upon.                        \
     */                                                                        \
    void list_##TNAME_SNAKE##_first(List##TNAME_PASCAL *list);                 \
                                                                               \
    /**                                                                        \
     * @brief Sets the active item to an item which comes after the active     \
     * item.                                                                   \
     *                                                                         \
     * @param list Pointer to the list to operate upon.                        \
     */                                                                        \
    void list_##TNAME_SNAKE##_next(List##TNAME_PASCAL *list);                  \
                                                                               \
    /**                                                                        \
     * @brief Via @p data returns the value of the first item in the list.     \
     *                                                                         \
     * @param list Pointer to the list to operate upon.                        \
     * @param data Pointer to the result variable.                             \
     * @return true On success.                                                \
     * @return false If list is empty.                                         \
     */                                                                        \
    bool list_##TNAME_SNAKE##_get_first(List##TNAME_PASCAL *list, T *data);    \
                                                                               \
    /**                                                                        \
     * @brief Via @p data returns the value of the active item in the list.    \
     *                                                                         \
     * @param list Pointer to the list to operate upon.                        \
     * @param data Pointer to the result variable.                             \
     * @return true On success.                                                \
     * @return false If list has no active member.                             \
     */                                                                        \
    bool list_##TNAME_SNAKE##_get_value(List##TNAME_PASCAL *list, T *data);    \
                                                                               \
    /**                                                                        \
     * @brief Overwrittes the value of the active item in the list.            \
     *                                                                         \
     * @param list Pointer to the list to operate upon.                        \
     * @param data Neew value of the active item.                              \
     */                                                                        \
    void list_##TNAME_SNAKE##_set_value(List##TNAME_PASCAL *list, T data);     \
                                                                               \
    /**                                                                        \
     * @brief Detects if a list item is active.                                \
     *                                                                         \
     * @param list Pointer to the list to operate upon.                        \
     * @return true If a item is active.                                       \
     * @return false If no item is active.                                     \
     */                                                                        \
    bool list_##TNAME_SNAKE##_is_active(List##TNAME_PASCAL *list);             \
                                                                               \
    /**                                                                        \
     * @brief Removes all items from the list and re-initializes the list.     \
     *                                                                         \
     * @param list Pointer to the list to operate upon.                        \
     */                                                                        \
    void list_##TNAME_SNAKE##_dispose(List##TNAME_PASCAL *list);

/*
 * Macro generating implementation of functions operating upon list.
 * 
 * @param T Data type of items stored in the list.
 * @param TNAME_PASCAL Any name for chosen data type in pascal case.
 * @param TNAME_SNAKE Any name for chosen data type in snake case.
 */

#define LIST_DEF(T, TNAME_PASCAL, TNAME_SNAKE)                                 \
                                                                               \
    void list_##TNAME_SNAKE##_init(List##TNAME_PASCAL *list) {				   \
        list->activeItem = NULL;											   \
        list->firstItem = NULL;												   \
    }																		   \
                                                                               \
    ListReturnCode list_##TNAME_SNAKE##_insert_first(List##TNAME_PASCAL *list, \
                                                    T data) {		   		   \
        ListItem##TNAME_PASCAL *item = malloc(sizeof *item);				   \
        if (item == NULL) return LIST_ALLOC_ERR;							   \
                                                                               \
        item->data = data;													   \
        item->nextItem = list->firstItem;									   \
                                                                               \
        list->firstItem = item;												   \
                                                                               \
        return LIST_OK;														   \
    }																		   \
                                                                               \
    ListReturnCode list_##TNAME_SNAKE##_insert_after(List##TNAME_PASCAL *list, \
                                                    T data) {		           \
        if (!list_##TNAME_SNAKE##_is_active(list)) return LIST_NOT_ACTIVE;	   \
                                                                               \
        ListItem##TNAME_PASCAL *item = malloc(sizeof *item);				   \
        if (item == NULL) return LIST_ALLOC_ERR;							   \
                                                                               \
        item->data = data;													   \
        item->nextItem = list->activeItem->nextItem;						   \
                                                                               \
        list->activeItem->nextItem = item;									   \
                                                                               \
        return LIST_OK;														   \
    }																		   \
                                                                               \
    ListReturnCode list_##TNAME_SNAKE##_insert_before(List##TNAME_PASCAL *list,\
                                                    T data) {		           \
        if (!list_##TNAME_SNAKE##_is_active(list)) return LIST_NOT_ACTIVE;	   \
                                                                               \
        ListItem##TNAME_PASCAL *item = malloc(sizeof *item);				   \
        if (item == NULL) return LIST_ALLOC_ERR;							   \
                                                                               \
        item->data = list->activeItem->data;								   \
        item->nextItem = list->activeItem->nextItem;						   \
                                                                               \
        list->activeItem->nextItem = item;									   \
        list->activeItem->data = data;										   \
                                                                               \
        list->activeItem = item;											   \
                                                                               \
        return LIST_OK;														   \
    }																		   \
                                                                               \
    void list_##TNAME_SNAKE##_delete_first(List##TNAME_PASCAL * list) {		   \
        if (list->firstItem == NULL) return;								   \
                                                                               \
        if (list->firstItem == list->activeItem) {							   \
            list->activeItem = NULL;										   \
        }																	   \
                                                                               \
        ListItem##TNAME_PASCAL *to_remove = list->firstItem;				   \
        list->firstItem = list->firstItem->nextItem;						   \
        free(to_remove);													   \
    }																		   \
                                                                               \
    void list_##TNAME_SNAKE##_delete_after(List##TNAME_PASCAL * list) {		   \
        if (!list_##TNAME_SNAKE##_is_active(list)							   \
            || list->activeItem->nextItem == NULL) { 						   \
            return;															   \
        }																	   \
                                                                               \
        ListItem##TNAME_PASCAL *to_remove = list->activeItem->nextItem;		   \
        list->activeItem->nextItem = list->activeItem->nextItem->nextItem;	   \
        free(to_remove);													   \
    }																		   \
                                                                               \
    void list_##TNAME_SNAKE##_first(List##TNAME_PASCAL *list) {				   \
        list->activeItem = list->firstItem;									   \
    }																		   \
                                                                               \
    void list_##TNAME_SNAKE##_next(List##TNAME_PASCAL * list) {				   \
        if (!list_##TNAME_SNAKE##_is_active(list)) {						   \
            return;															   \
        }																	   \
                                                                               \
        list->activeItem = list->activeItem->nextItem;						   \
    }																		   \
                                                                               \
    bool list_##TNAME_SNAKE##_get_first(List##TNAME_PASCAL *list, T *data) {   \
        if (list->firstItem == NULL) return false;							   \
                                                                               \
        *data = list->firstItem->data;										   \
                                                                               \
        return true;														   \
    }																		   \
                                                                               \
    bool list_##TNAME_SNAKE##_get_value(List##TNAME_PASCAL *list, T *data) {   \
        if (!list_##TNAME_SNAKE##_is_active(list)) {						   \
            return false;													   \
        }																	   \
                                                                               \
        *data = list->activeItem->data;										   \
                                                                               \
        return true;														   \
    }																		   \
                                                                               \
    void list_##TNAME_SNAKE##_set_value(List##TNAME_PASCAL *list, T data) {	   \
        if (!list_##TNAME_SNAKE##_is_active(list)) {						   \
            return;															   \
        }																	   \
                                                                               \
        list->activeItem->data = data;										   \
        return;																   \
    }																		   \
                                                                               \
    bool list_##TNAME_SNAKE##_is_active(List##TNAME_PASCAL *list) {			   \
        return list->activeItem != NULL;									   \
    }																		   \
   \
    void list_##TNAME_SNAKE##_dispose(List##TNAME_PASCAL *list) {			   \
        list_##TNAME_SNAKE##_first(list);									   \
                                                                               \
        ListItem##TNAME_PASCAL *to_remove = list->activeItem->nextItem;		   \
        while (list->firstItem != NULL)										   \
        {																	   \
            to_remove = list->firstItem;									   \
            list->firstItem = list->firstItem->nextItem;					   \
            free(to_remove);												   \
        }																	   \
                                                                               \
        list->activeItem = NULL;											   \
        list->firstItem = NULL;												   \
    }

#endif /* LIST_H */