#pragma once

#include <stdint.h>

#include "string.h"
#include "bucket.h"
#include "llist.h"
#include "mem.h"

typedef uint32_t hash_t;

struct _hashmap_item {
    char* key;
    void* value;
};

struct _hashmap_item* _hashmap_item_create(const char* key, void* value);
void _hashmap_item_destroy(struct _hashmap_item*);

typedef struct {
    bucket(llist(struct _hashmap_item*)) items;

    size_t item_count;
} hashmap;

void hashmap_new(hashmap*);

void hashmap_del(hashmap*);

///returns: the number of items in the hashmap
size_t hashmap_item_count(hashmap*);

///goes through each item in the hashmap, and calls a function so that you can delete the value
///in the process also deletes the hashmap
void hashmap_del_each(hashmap*, void(*)(void*));

///if item is not found, return NULL
void* hashmap_get(hashmap*, const char* key);

///WARNING: this function calls strlen() on key, be sure to do input validation on key
///NOTE: the data in key is coppied
void hashmap_set(hashmap*, const char* key, void* value);

///key MUST be allocated with string_new2()
///ownership is handed to hashmap after this call
///NOTE: the data in key is coppied
void hashmap_set_safe(hashmap*, moved(string*) key, void*);

///returns -1 if the key is not in the map
int hashmap_unset(hashmap*, const char* key);

///checks if key exists in the map
bool hashmap_exists(hashmap*, const char* key);

hash_t hash_str(const char* str);

void hashmap_foreach(hashmap*, void(*)(void*));
