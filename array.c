#include <stdlib.h>
#include <string.h>
#include "array.h"

array* array_new2(size_t cap, size_t item_size)
{
    array* arr = malloc(sizeof(array));
    arr->len = 0;

    if(cap == 0) cap = 1;

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

int array_append(array* arr, void* data) {
    if (arr->len == arr->cap) {
        void* buf = realloc(arr->data, arr->cap * 2 * arr->item_size);
        if (buf == NULL) {
            return -1;
        }
        arr->data = buf;

        arr->cap *= 2;
    }

    memcpy(arr->data + arr->len * arr->item_size, data, arr->item_size);
    arr->len++;

    return 0;
}

void* array_at(array* arr, size_t idx) {
    if(arr->len == 0) {
        return NULL;
    }
    if (idx >= arr->len) {
        return NULL;
    }
    return arr->data + idx * arr->item_size;
}

size_t array_len(array* arr) {
    return arr->len;
}
