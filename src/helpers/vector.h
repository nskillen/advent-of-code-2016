#ifndef __VECTOR_H__
#define __VECTOR_H__

#include <stddef.h>

#include "callback_types.h"

typedef struct {
  /* data */
  int auto_grow;
  size_t max_count;
  size_t element_count;
  size_t element_size;
  void *data;

  /* func pointers */
  comparison_callback comp;
} vector;

vector* vector_create(size_t element_size, size_t initial_count, int auto_grow, comparison_callback callback);
void vector_destroy(vector *v);
int vector_grow(vector *v, float growth_factor);

int   vector_insert (vector *v, size_t index, void *data);
void* vector_remove (vector *v, size_t index);
void* vector_get    (vector *v, size_t index);

int   vector_push   (vector *v, void *data);
void* vector_pop    (vector *v);

int   vector_enqueue (vector *v, void *data);
void* vector_dequeue (vector *v);

vector* vector_select(vector *v, int (*selector)(void*));
vector* vector_reject(vector *v, int (*selector)(void*));

#endif//__VECTOR_H__
