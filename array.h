/*
array.h

The array struct is mostly useful for pointer objects as opposed to intrinsic
values such as `3`.
It can be used for intrinsic values but the type of the array must be a pointer
to said type
*/

#pragma once
#include <stddef.h>
#include <stdint.h>
#include "iter.h"

typedef struct {
    size_t len;
    size_t cap;
    size_t item_size;
    void* data;

    size_t iter_pos;
} array;

#define array_t(ty) array

///creates a new array and returns the pointer
///caller must call array_del
array* array_new2(size_t cap, size_t item_size);

void array_del2(array*);

///returns -1 on fail
///
///WARNING: does not create a copy of data
///         if data is freed, trying to access it from the array
///         will cause a segfault.
int array_append(array*, void* data);

///returns a pointer to an item in the array
///returns NULL on out of bounds
void** array_at(array*, size_t idx);

///creates a copy of an item in the array in out
///out will be set to NULL if idx is out of bounds or longer than len
void array_get(array*, size_t, void* out);

size_t array_len(array*);

void array_remove_idx(array*, size_t idx);

void array_clear(array*);

///finds the pointer p in the array (not the value of what p points to)
///returns (size_t)-1 if not found
size_t array_find(array*, void* p);

void* array_next(array*);

void array_iter(array*, struct iterable_t*);

typedef struct {
    array* arr;
    uint8_t byte_delim;
} array_stream_splitter;

// splites bytes from a stream and saves them as string*s in the provided
// array_stream_splitter->arr.
size_t array_stream_split_writer(array_stream_splitter* info,
        uint8_t* buf, size_t bufsize);
