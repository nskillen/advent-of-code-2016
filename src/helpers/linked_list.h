#ifndef __LINKED_LIST_H__
#define __LINKED_LIST_H__

#include "callback_types.h"

typedef enum { BEFORE, AFTER } relative_position;

typedef struct _list_node {
	void *data;
	size_t data_len;
	struct _list_node *next;
	struct _list_node *prev;
} list_node;

typedef struct {
	list_node *front;
	list_node *back;
	deletion_callback deleter;
	comparison_callback comparator;
	size_t length;
} linked_list;

linked_list* create_list(deletion_callback del_callback, comparison_callback comp_callback);
void destroy_list(linked_list *list);
int list_push(linked_list *list, void *data, size_t data_len);
int list_insert_at(linked_list *list, size_t position, void *data, size_t data_len);
int list_remove(linked_list *list, void *data);
int list_contains(linked_list *list, void* data);
void* list_get_at_position(linked_list *list, size_t position);
void list_each(linked_list *list, iterator_callback callback);
void list_each_with_index(linked_list *list, indexed_iterator_callback callback);
void list_move_element(linked_list *list, void *data, void *tgt_data, relative_position insert_pos);

#endif//__LINKED_LIST_H__
