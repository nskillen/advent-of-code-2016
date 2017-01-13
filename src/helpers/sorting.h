#ifndef __SORTING_H__
#define __SORTING_H__

#include "linked_list.h"
#include "vector.h"

/* List sorts */

void l_insertion_sort(linked_list *list);
void l_selection_sort(linked_list *list);
void l_bubble_sort(linked_list *list);
void l_quick_sort(linked_list *list);
void l_merge_sort(linked_list *list);

/* Vector sorts */

void v_insertion_sort(vector *v);
void v_selection_sort(vector *v);
void v_bubble_sort(vector *v);
void v_quick_sort(vector *v);
void v_merge_sort(vector *v);

#endif//__SORTING_H__
