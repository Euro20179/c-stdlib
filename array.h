#pragma once
#include <stddef.h>

typedef struct {
    size_t len;
    size_t cap;
    size_t item_size;
    void* data;
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
