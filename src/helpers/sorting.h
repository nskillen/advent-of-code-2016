#ifndef __SORTING_H__
#define __SORTING_H__

#include "callback_types.h"
#include "linked_list.h"

void insertion_sort(linked_list *list);
void selection_sort(linked_list *list);
void bubble_sort(linked_list *list);
void quick_sort(linked_list *list);
void merge_sort(linked_list *list);

#endif//__SORTING_H__
