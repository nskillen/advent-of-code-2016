#ifndef __CALLBACK_TYPES_H__
#define __CALLBACK_TYPES_H__

#include <stddef.h>

struct _linked_list;

// returns -1 when first arguments is less than second, 0 when equal, 1 when greater
typedef int  (*comparison_callback)        (void*, void*);

typedef void (*deletion_callback)          (void*);

typedef void (*sorting_callback)           (struct _linked_list *, comparison_callback);

typedef void (*iterator_callback)          (void*, size_t);

typedef void (*indexed_iterator_callback)  (size_t,void*,size_t);
#endif//__CALLBACK_TYPES_H__
