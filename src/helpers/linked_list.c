#include "linked_list.h"
#include "callback_types.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/** Helper Functions **/

static list_node* _create_node(void *data, size_t data_len) {
  list_node *n = (list_node*)calloc(1, sizeof(list_node));
  if (n == NULL) { return NULL; }

  n->data_len = data_len;
  n->data = malloc(data_len);
  if (n->data == NULL) {
    free(n);
    return NULL;
  }
  memcpy(n->data, data, data_len);

  return n;
}

static list_node* find(linked_list *list, void *data) {
	list_node *n = list->front;

	while (n != NULL) {
		if (list->comparator != NULL && list->comparator(data, n->data)) { break; }
		else if (data == n->data) { break; }

		n = n->next;
	}

	return n;
}

/* Public list functions */

linked_list* create_list(deletion_callback deleter, comparison_callback comparator) {
	linked_list* ll = (linked_list*)calloc(1, sizeof(linked_list));
	if (ll == NULL) { return NULL; }

	ll->length = 0;

	ll->deleter = deleter;
	ll->comparator = comparator;

	return ll;
}

void destroy_list(linked_list *list) {
	while (list->front != NULL) {
		list_node *n = list->front;
		if (list->deleter) { list->deleter(n->data); }
		if (n->data) { free(n->data); }
		list->front = list->front->next;
		free(n);
	}
	free(list);
}

int list_push(linked_list *list, void* data, size_t data_len) {
  list_node *new_node = _create_node(data, data_len);
  if (new_node == NULL) { return 0; }

  if (list->front == NULL) {
    list->front = new_node;
    list->back = new_node;
  } else {
    new_node->prev = list->back;
    list->back->next = new_node;
    list->back = new_node;
  }
  list->length += 1;
  return 1;
}

int list_insert_at(linked_list *list, size_t position, void *data, size_t data_len) {
  /* can insert AT end of list, but not PAST end of list */
  if (list->length < position) { return 0; }
  list_node *node_at_position = list->front;
  for (size_t i = 0; i < position; i++) {
    node_at_position = node_at_position->next;
    if (i+1 < position && node_at_position == NULL) {
      /* broken link, entire list is fucked */
      return 0;
    }
  }

	list_node *new_node = _create_node(data, data_len);
	if (new_node == NULL) { return 0; }

	new_node->next = NULL;
	new_node->prev = NULL;

	if (node_at_position == NULL) {
    /* requested position is at end of list */
    new_node->prev = list->back;
    list->back->next = new_node;
    list->back = new_node;
	} else {
    new_node->prev = node_at_position->prev;
    new_node->next = node_at_position;

    node_at_position->prev->next = new_node;
    node_at_position->prev = new_node;
	}

	list->length += 1;

	return 1;
}

void* list_pop(linked_list *list) {
  if (list->back == NULL) { return NULL; }

  list_node *n = list->back;
  list->back = list->back->prev;
  if (list->back == NULL) { list->front = NULL; }
  list->length -= 1;

  void *data = n->data;
  n->data = NULL;
  free(n);

  return data;
}

void* list_get_at_position(linked_list *list, size_t position) {
	list_node *n = list->front;
	for (size_t p = 0; p <= position && n != NULL; p++) {
		n = n->next;
	}
	if (n == NULL) { return NULL; }
	return n->data;
}

int list_remove(linked_list *list, void *data) {
	if (list->front == NULL) { return 0; }

	list_node *prev_node = NULL;
	list_node *cur_node = list->front;

	while (cur_node != NULL &&
			(cur_node->data != data ||
				(list->comparator != NULL && list->comparator(data, cur_node->data) == 0))) {
		prev_node = cur_node;
		cur_node = cur_node->next;
	}
	
	if (cur_node == NULL) { return 0; }

	prev_node->next = cur_node->next;

	if (list->deleter) { list->deleter(cur_node->data); }

	if (cur_node->data) { free(cur_node->data); }
	free(cur_node);

	list->length -= 1;

	return 1;
}

int list_contains(linked_list *list, void* data) {
	return find(list, data) != NULL;
}

void list_each(linked_list *list, iterator_callback iterator) {
	list_node *n = list->front;
	while (n != NULL) {
		iterator(n->data, n->data_len);
		n = n->next;
	}
}

void list_each_with_index(linked_list *list, indexed_iterator_callback iterator) {
	list_node *n = list->front;
	int index = 0;
	while (n != NULL) {
		iterator(index, n->data, n->data_len);
		n = n->next;
		index += 1;
	}
}

void list_move_element(linked_list *list, void *data, void *tgt_data, relative_position insert_pos) {
	list_node *from = find(list, data);
	list_node *to = find(list, tgt_data);

	if (from == NULL || to == NULL) { return; }

	if (list->front == from) { list->front = from->next; }
	if (list->back == from) { list->back = from->prev; }
	if (from->prev) { from->prev->next = from->next; }
	if (from->next) { from->next->prev = from->prev; }

	switch (insert_pos) {
		case BEFORE:
			from->prev = to->prev;
			from->next = to;

			if (to->prev) { to->prev->next = from; }
			to->prev = from;
			
			break;

		case AFTER:
			from->next = to->next;
			from->prev = to;

			if (to->next) { to->next->prev = from; }
			to->next = from;

			break;
	}
}

