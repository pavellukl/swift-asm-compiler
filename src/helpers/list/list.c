#include "list.h"

/*
 * Macro generating implementation of functions operating upon list.
 * 
 * @param T Data type of items stored in the list.
 * @param TNAME_PASCAL Any name for chosen data type in pascal case.
 * @param TNAME_SNAKE Any name for chosen data type in snake case.
 */
#define LIST_DEF(T, TNAME_PASCAL, TNAME_SNAKE)                                 \
																			   \
void list_##TNAME_SNAKE##_init(List##TNAME_PASCAL *list) {					   \
	list->activeItem = NULL;												   \
	list->firstItem = NULL;													   \
}																			   \
																			   \
ListReturnCode list_##TNAME_SNAKE##_insert_first(List##TNAME_PASCAL *list,     \
												 T data) {		   			   \
	ListItem##TNAME_PASCAL *item = malloc(sizeof *item);				   	   \
	if (item == NULL) return LIST_ALLOC_ERR;								   \
																			   \
	item->data = data;														   \
	item->nextItem = list->firstItem;										   \
																			   \
	list->firstItem = item;													   \
																			   \
	return LIST_OK;															   \
}																			   \
																			   \
ListReturnCode list_##TNAME_SNAKE##_insert_after(List##TNAME_PASCAL *list,     \
												 T data) {		               \
	if (!list_##TNAME_SNAKE##_is_active(list)) return LIST_NOT_ACTIVE;		   \
																			   \
	ListItem##TNAME_PASCAL *item = malloc(sizeof *item);				       \
	if (item == NULL) return LIST_ALLOC_ERR;								   \
																			   \
	item->data = data;														   \
	item->nextItem = list->activeItem->nextItem;							   \
																			   \
	list->activeItem->nextItem = item;										   \
																			   \
	return LIST_OK;															   \
}																			   \
																			   \
ListReturnCode list_##TNAME_SNAKE##_insert_before(List##TNAME_PASCAL *list,    \
												  T data) {		               \
	if (!list_##TNAME_SNAKE##_is_active(list)) return LIST_NOT_ACTIVE;		   \
																			   \
	ListItem##TNAME_PASCAL *item = malloc(sizeof *item);				       \
	if (item == NULL) return LIST_ALLOC_ERR;								   \
																			   \
	item->data = list->activeItem->data;									   \
	item->nextItem = list->activeItem->nextItem;							   \
																			   \
	list->activeItem->nextItem = item;										   \
	list->activeItem->data = data;											   \
																			   \
	list->activeItem = item;												   \
																			   \
	return LIST_OK;															   \
}																			   \
																			   \
void list_##TNAME_SNAKE##_delete_first(List##TNAME_PASCAL * list) {			   \
	if (list->firstItem == NULL) return;									   \
																			   \
	if (list->firstItem == list->activeItem) {								   \
		list->activeItem = NULL;											   \
	}																		   \
																			   \
	ListItem##TNAME_PASCAL *to_remove = list->firstItem;					   \
	list->firstItem = list->firstItem->nextItem;							   \
	free(to_remove);														   \
}																			   \
																			   \
void list_##TNAME_SNAKE##_delete_after(List##TNAME_PASCAL * list) {			   \
	if (!list_##TNAME_SNAKE##_is_active(list)								   \
		|| list->activeItem->nextItem == NULL) { 							   \
		return;																   \
	}																		   \
																			   \
	ListItem##TNAME_PASCAL *to_remove = list->activeItem->nextItem;			   \
	list->activeItem->nextItem = list->activeItem->nextItem->nextItem;		   \
	free(to_remove);														   \
}																			   \
																			   \
void list_##TNAME_SNAKE##_first(List##TNAME_PASCAL *list) {					   \
	list->activeItem = list->firstItem;										   \
}																			   \
																			   \
void list_##TNAME_SNAKE##_next(List##TNAME_PASCAL * list) {					   \
	if (!list_##TNAME_SNAKE##_is_active(list)) {							   \
		return;																   \
	}																		   \
																			   \
	list->activeItem = list->activeItem->nextItem;							   \
}																			   \
																			   \
bool list_##TNAME_SNAKE##_get_first(List##TNAME_PASCAL *list, T *data) {	   \
	if (list->firstItem == NULL) return false;								   \
																			   \
	*data = list->firstItem->data;											   \
																			   \
	return true;															   \
}																			   \
																			   \
bool list_##TNAME_SNAKE##_get_value(List##TNAME_PASCAL *list, T *data) {	   \
	if (!list_##TNAME_SNAKE##_is_active(list)) {							   \
		return false;														   \
	}																		   \
																			   \
	*data = list->activeItem->data;											   \
																			   \
	return true;															   \
}																			   \
																			   \
void list_##TNAME_SNAKE##_set_value(List##TNAME_PASCAL *list, T data) {		   \
	if (!list_##TNAME_SNAKE##_is_active(list)) {							   \
		return;																   \
	}																		   \
																			   \
	list->activeItem->data = data;											   \
	return;																	   \
}																			   \
																			   \
bool list_##TNAME_SNAKE##_is_active(List##TNAME_PASCAL *list) {				   \
	return list->activeItem != NULL;										   \
}																			   \
																			   \
void list_##TNAME_SNAKE##_dispose(List##TNAME_PASCAL *list) {				   \
	list_##TNAME_SNAKE##_first(list);										   \
																			   \
	ListItem##TNAME_PASCAL *to_remove = list->activeItem->nextItem;			   \
	while (list->firstItem != NULL)											   \
	{																		   \
		to_remove = list->firstItem;										   \
		list->firstItem = list->firstItem->nextItem;						   \
		free(to_remove);													   \
	}																		   \
																			   \
	list->activeItem = NULL;												   \
	list->firstItem = NULL;													   \
}

LIST_DEF(PPListItem, PP, pp)
LIST_DEF(ListElement*, ST, st)
