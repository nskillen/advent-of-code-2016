#ifndef __LINKED_LIST_H__
#define __LINKED_LIST_H__

#include "callback_types.h"

#define LIST_FN(n, r, ...) r list_##n (linked_list *list, ##__VA_ARGS__)

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

LIST_FN(push,            int,   void *data, size_t data_len);
LIST_FN(insert_at,       int,   size_t position, void *data, size_t data_len);
LIST_FN(pop,             void*);
LIST_FN(get_at_position, void*, size_t position);
LIST_FN(move_element,    void,  void *data, void *tgt_data, relative_position insert_pos);
LIST_FN(remove,          int,   void *data);
LIST_FN(contains,        int,   void *data);
LIST_FN(each,            void,  iterator_callback callback);
LIST_FN(each_with_index, void,  indexed_iterator_callback callback);

/*
int   list_push            (linked_list *list, void *data, size_t data_len);
int   list_insert_at       (linked_list *list, size_t position, void *data, size_t data_len);

void* list_pop             (linked_list *list);
void* list_get_at_position (linked_list *list, size_t position);

void  list_move_element    (linked_list *

int   list_remove          (linked_list *list, void *data);

int   list_contains        (linked_list *list, void* data);

void  list_each            (linked_list *list, iterator_callback callback);
void  list_each_with_index (linked_list *list, indexed_iterator_callback callback);
*/
#endif//__LINKED_LIST_H__
