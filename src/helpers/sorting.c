#include "sorting.h"

#include <stdlib.h>

void insertion_sort(linked_list *list) {
	list_node *n = list->front->next;
	list_node *np = n->prev;
	list_node *nn = n->next;

	while(n != NULL) {
		while (list->comparator(n->data, np->data) < 0) { np = np->prev; }
		list_move_element(list, n, np, AFTER);
		n = nn;
		np = n->prev;
		nn = n->next;
	}
}

void selection_sort(linked_list *list) {
	
}

void bubble_sort(linked_list *list) {
	
}

void quick_sort(linked_list *list) {
	
}

void merge_sort(linked_list *list) {
	
}

