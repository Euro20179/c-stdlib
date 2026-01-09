#include "array.h"
#include "string.h"
#include "iter.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

array* array_new2(size_t cap, size_t item_size)
{
    array* arr = malloc(sizeof(array));
    arr->len = 0;

    if (cap == 0)
        cap = 1;

    arr->cap = cap;
    arr->item_size = item_size;
    arr->data = calloc(cap, item_size);

    return arr;
}

void array_del2(array* arr)
{
    free(arr->data);
    free(arr);
}

int array_append(array* arr, void* data)
{
    if (arr->len == arr->cap) {
        void* buf = realloc(arr->data, arr->cap * 2 * arr->item_size);
        if (buf == NULL) {
            return -1;
        }
        arr->data = buf;

        arr->cap *= 2;
    }

    *((void**)(arr->data + arr->len * arr->item_size)) = data;
    arr->len++;

    return 0;
}

void** array_at(array* arr, size_t idx)
{
    if (arr->len == 0) {
        return NULL;
    }
    if (idx >= arr->len) {
        return NULL;
    }
    return arr->data + idx * arr->item_size;
}

void array_get(array* arr, size_t idx, void* out) {
    if (arr->len == 0) {
        out = NULL;
    }
    if (idx >= arr->len) {
        out = NULL;
    }

    memcpy(out, arr->data + idx * arr->item_size, arr->item_size);
}

size_t array_len(array* arr)
{
    return arr->len;
}

void array_remove_idx(array* arr, size_t idx)
{
    size_t items_to_move = array_len(arr) - idx;
    memcpy(arr->data + idx * arr->item_size, arr->data + (idx + 1) * arr->item_size, arr->item_size * items_to_move);
    arr->len--;
}

void array_clear(array* arr)
{
    arr->len = 0;
}

size_t array_find(array* arr, void* p)
{
    size_t len = array_len(arr);
    for (size_t i = 0; i < len; i++) {
        if (array_at(arr, i) == p) {
            return i;
        }
    }
    return -1;
}

void array_iter(array* arr, struct iterable_t* i)
{
    ((array*)arr)->iter_pos = 0;
}

void* array_next(array* arr)
{
    return array_at(arr, ((array*)arr)->iter_pos++);
}

size_t array_stream_split_writer(array_stream_splitter* info,
        uint8_t* buf, size_t bufsize) {
    array* arr = info->arr;

    //bootstrap
    if(array_len(arr) == 0) {
        array_append(arr, string_new2(1));
    }

    string* cur_string = *(string**)array_at(arr, array_len(arr) - 1);
    for(size_t i = 0; i < bufsize; i++) {
        if (buf[i] == info->byte_delim) {
            cur_string = string_new2(1);
            array_append(arr, cur_string);
            continue;
        }

        string_concat_char(cur_string, buf[i]);
    }
    return bufsize;
}
