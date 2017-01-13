#include "vector.h"

#include <stdlib.h>
#include <string.h>

/* Private helper methods */
static size_t offset(vector *v, size_t index) {
  return v->element_size * index;
}

static void* elem_ptr(vector *v, size_t index) {
  return v->data + offset(v, index);
}

vector* vector_create(size_t element_size, size_t initial_count, int auto_grow, comparison_callback callback) {
  vector *v = (vector*)calloc(1, sizeof(vector));
  if (NULL == v) { return NULL; } /* failed to allocate vector */
  if (0 == initial_count) { return NULL; }

  v->element_size = element_size;
  v->element_count = 0;
  v->max_count = initial_count;
  v->auto_grow = auto_grow;

  v->data = (void*)calloc(initial_count, element_size);
  if (NULL == v->data) { free(v); return NULL; }

  v->comp = callback;

  return v;
}

void vector_destroy(vector *v) {
  free(v->data);
  free(v);
}

int vector_grow(vector *v, float growth_factor) {
  if (NULL == v) { return 0; }

  void *old_data = v->data;

  size_t new_max = v->max_count * growth_factor;
  v->data = (void*)calloc(new_max * v->element_size, sizeof(char));
  if (NULL == v->data) { v->data = old_data; return 0; }

  for (size_t i = 0; i < new_max; i++) {
    size_t off = offset(v, i);
    memcpy(elem_ptr(v, i), old_data + off, v->element_size);
  }

  v->max_count = new_max;
  if (new_max < v->element_count) { v->element_count = new_max; }

  free(old_data);
  return 1;
}

int vector_insert(vector *v, size_t index, void *data) {
  if (NULL == v) { return 0; }
  if (NULL == data) { return 0; }

  if (v->element_count + 1 >= v->max_count) {
    if (!v->auto_grow) { return 0; }
    if (!vector_grow(v, 2)) { return 0; }
  }

  for (size_t i = v->element_count; i > index; i--) {
    memcpy(elem_ptr(v, i), elem_ptr(v, i - 1), v->element_size);
  }

  memcpy(elem_ptr(v, index), data, v->element_size);

  v->element_count += 1;

  return 1;
}

void* vector_remove(vector *v, size_t index) {
  if (NULL == v) { return NULL; }
  if (index+1 > v->element_count) { return NULL; }

  void *elem = vector_get(v, index);
  if (NULL == elem) { return NULL; }

  for (size_t i = index; i < v->element_count; i++) {
    memcpy(elem_ptr(v, i), elem_ptr(v, i + 1), v->element_size);
  }

  v->element_count -= 1;

  return elem;
}

void* vector_get(vector *v, size_t index) {
  if (NULL == v) { return NULL; }
  if (index + 1 > v->element_count) { return NULL; }

  void *elem = (void*)calloc(1, v->element_size);
  if (NULL == elem) { return NULL; }

  memcpy(elem, elem_ptr(v, index), v->element_size);

  return elem;
}

int vector_push(vector *v, void *data) {
  if (NULL == v) { return 0; }

  return vector_insert(v, v->element_count, data);
}

void* vector_pop(vector *v) {
  if (NULL == v) { return 0; }
  return vector_remove(v, v->element_count - 1);
}

int vector_enqueue(vector *v, void *data) {
  return vector_insert(v, 0, data);
}

void* vector_dequeue(vector *v) {
  return vector_remove(v, 0);
}

vector* vector_select(vector *v, int (*selector)(void*)) {
  vector *out = vector_create(v->element_size, v->max_count / 2, v->auto_grow, v->comp);
  if (out == NULL) { return NULL; }
  
  for (size_t i = 0; i < v->element_count; i++) {
    if (selector(elem_ptr(v, i))) { vector_push(out, elem_ptr(v, i)); }
  }

  return out;
}

vector* vector_reject(vector *v, int (*selector)(void*)) {
  vector *out = vector_create(v->element_size, v->max_count / 2, v->auto_grow, v->comp);
  if (out == NULL) { return NULL; }
  
  for (size_t i = 0; i < v->element_count; i++) {
    if (!selector(elem_ptr(v, i))) { vector_push(out, elem_ptr(v, i)); }
  }

  return out;
}
