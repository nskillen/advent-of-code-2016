#include "sorting.h"

#include <stdio.h>
#include <stdlib.h>

#ifdef DEBUG
static int debug=1;
#else
static int debug=0;
#endif


/* List sorts */

void l_insertion_sort(linked_list *list) {
  fprintf(stderr, "Not yet implemented, list was 0x%X\n", (unsigned int)list);
  exit(1);
}

void l_selection_sort(linked_list *list) {
  fprintf(stderr, "Not yet implemented, list was 0x%X\n", (unsigned int)list);
  exit(1);
}

void l_bubble_sort(linked_list *list) {
  fprintf(stderr, "Not yet implemented, list was 0x%X\n", (unsigned int)list);
  exit(1);
}

void l_quick_sort(linked_list *list) {
  fprintf(stderr, "Not yet implemented, list was 0x%X\n", (unsigned int)list);
  exit(1);
}

void l_merge_sort(linked_list *list) {
  fprintf(stderr, "Not yet implemented, list was 0x%X\n", (unsigned int)list);
  exit(1);
}

/* Vector sorts */

char* elem_ptr(vector *v, size_t index) {
  if (NULL == v) { return NULL; }
  if (index >= v->element_count) { return NULL; }
  return v->data + (index * v->element_size);
}

void v_insertion_sort(vector *v) {
  if (NULL == v || v->element_count < 2) { return; }

  size_t cur_idx = 1;
  size_t sorted_idx = 0;
  size_t tgt_idx = 0;

  char *a = NULL;
  char *b = NULL;
  char *tmp = NULL;

  while (cur_idx < v->element_count) {
    a = elem_ptr(v, cur_idx);
    b = elem_ptr(v, sorted_idx);

    if (debug && b == NULL) { goto printerr; }

    if (v->comp(a, b) < 0) {
      if (sorted_idx > 0) {
        tgt_idx = sorted_idx - 1;
        for (; tgt_idx > 0; tgt_idx--) {
          b = elem_ptr(v, tgt_idx);
          if (b == NULL) { goto printerr; }
          if (v->comp(a, b) >= 0) { break; }
        }
      } else {
        tgt_idx = 0;
      }

      if (tgt_idx == 0 && v->comp(a, v->data) < 0) {
        /* need to insert before 0 */
        tmp = vector_remove(v, cur_idx);
        vector_enqueue(v, tmp);
      } else {
        /* need to insert after tgt_idx */
        tmp = vector_remove(v, cur_idx);
        vector_insert(v, tgt_idx + 1, tmp);
      }
      if (tmp) { free(tmp); tmp = NULL; }
    }
    
    sorted_idx += 1;
    cur_idx = sorted_idx + 1;
  }
  return;
printerr:
  fprintf(stderr, "ERROR during vector insertion sort\n");
  fprintf(stderr, "Arguments:\n");
  fprintf(stderr, "\tv:     vector{ element_size: %zu, element_count: %zu max_count: %zu, auto_grow: %d }\n", v->element_size, v->element_count, v->max_count, v->auto_grow);
  fprintf(stderr, "Locals:\n");
  fprintf(stderr, "\tcur_idx:    %zu\n", cur_idx);
  fprintf(stderr, "\tsorted_idx: %zu\n", sorted_idx);
  fprintf(stderr, "\ttgt_idx:    %zu\n", tgt_idx);
  fprintf(stderr, "\ta:          0x%X\n", (unsigned int)a);
  fprintf(stderr, "\tb:          0x%X\n", (unsigned int)b);
  fprintf(stderr, "\ttmp:        0x%X\n", (unsigned int)tmp);
  exit(1);
}

void v_selection_sort(vector *v) {
  fprintf(stderr, "Not yet implemented, vector was 0x%X\n", (unsigned int)v);
  exit(1);
}

void v_bubble_sort(vector *v) {
  fprintf(stderr, "Not yet implemented, vector was 0x%X\n", (unsigned int)v);
  exit(1);
}

void v_quick_sort(vector *v) {
  fprintf(stderr, "Not yet implemented, vector was 0x%X\n", (unsigned int)v);
  exit(1);
}

void v_merge_sort(vector *v) {
  fprintf(stderr, "Not yet implemented, vector was 0x%X\n", (unsigned int)v);
  exit(1);
}

