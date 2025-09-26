#pragma once
#include <stddef.h>
#include "iter.h"

typedef struct {
    size_t len;
    size_t cap;
    size_t item_size;
    void* data;

    size_t iter_pos;
} array;

///creates a new array and returns the pointer
///caller must call array_del
array* array_new2(size_t cap, size_t item_size);

void array_del2(array*);

///returns -1 on fail
int array_append(array*, void* data);

///returns NULL on out of bounds
void* array_at(array*, size_t idx);

size_t array_len(array*);

void array_remove_idx(array*, size_t idx);

void array_clear(array*);

///finds the pointer p in the array (not the value of what p points to)
///returns (size_t)-1 if not found
size_t array_find(array*, void* p);

void* array_next(array*);

void array_iter(array*, struct iterable_t*);
