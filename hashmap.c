#include "hashmap.h"
#include "bucket.h"
#include "llist.h"
#include "mem.h"
#include "string.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct _hashmap_item* _hashmap_item_create(const char* key, void* value)
{
    struct _hashmap_item* item = malloc(sizeof(struct _hashmap_item));
    int len = strlen(key);
    item->key = calloc(len, 1);
    memcpy(item->key, key, len);
    item->value = value;
    return item;
}

size_t collide(size_t idx, size_t maxlen)
{
    return (idx + 1) % maxlen;
}

void _hashmap_item_destroy(struct _hashmap_item* item)
{
    free(item->key);
    free(item);
}

void hashmap_new(hashmap* map)
{
    bucket_new(&map->items, 256, llist(struct _hashmap_item*));
}

void hashmap_del(hashmap* map)
{
    bucket_del(&map->items);
}

void hashmap_del_each(hashmap* map, void(cb)(void*))
{
    for (int i = 0; i < bucket_size(&map->items); i++) {
        if (!bucket_full_at(&map->items, i))
            continue;

        llist* ref = (llist*)bucket_get(&map->items, i);
        llist_node* cur = ref->head;
        while (cur != NULL) {
            cb(((struct _hashmap_item*)cur->data)->value);
            cur = cur->next;
        }
        llist_del(ref);
    }
    bucket_del(&map->items);
}

size_t get_idx_from_hash(hashmap* map, hash_t h)
{
    return h % bucket_size(&map->items);
}

void hashmap_set_safe(hashmap* map, moved(string*) key, void* value)
{
    const char* data = string_mkcstr(key);
    hashmap_set(map, data, value);
    string_del2(key);
}

void hashmap_set(hashmap* map, const char* key, void* value)
{
    hash_t h = hash_str(key);

    struct _hashmap_item* item = _hashmap_item_create(key, value);

    size_t idx = get_idx_from_hash(map, h);

    size_t initial_idx = idx;

    llist* ref;

    if (!bucket_full_at(&map->items, idx)) {
        llist* i = llist_new2();
        llist_append(i, item);
        bucket_set(&map->items, idx, i);
    } else {
        ref = (llist*)bucket_get(&map->items, idx);

        llist_node* cur = llist_next(ref, NULL);

        while(cur != NULL) {
            struct _hashmap_item* old_item = llist_node_get(cur);
            if (strcmp(old_item->key, key) == 0) {
                _hashmap_item_destroy(old_item);
                llist_node_set(cur, item);
                //we are completely done,
                //there are no new items, so dont inc item_count,
                //and dont append a new item
                return;
            }
            cur = llist_next(ref, cur);
        }

        llist_append(ref, item);
    }
    map->item_count++;
}

int key_cmp(const char* key1, const char* key2)
{
    return strcmp(key1, key2);
}

int hashmap_unset(hashmap* map, const char* key)
{
    hash_t h = hash_str(key);
    size_t idx = get_idx_from_hash(map, h);

    llist item = *(llist*)bucket_get(&map->items, idx);
    if (item.len) {
        return -1;
    }

    llist_node* cur = item.head;
    size_t llist_idx = 0;
    while (key_cmp(((struct _hashmap_item*)cur->data)->key, key) != 0 && cur->next != NULL) {
        cur = cur->next;
        llist_idx++;
    }

    _hashmap_item_destroy(cur->data);
    llist_remove(&item, llist_idx);
    if (item.len == 0) {
        llist_del(&item);
        bucket_remove(&map->items, idx);
    }

    map->item_count--;
    return 0;
}

void* hashmap_get(hashmap* map, const char* key)
{
    hash_t h = hash_str(key);
    size_t idx = h % bucket_size(&map->items);
    llist item = *(llist*)bucket_get(&map->items, idx);

    llist_node* cur = item.head;
    while (cur != NULL && key_cmp(((struct _hashmap_item*)cur->data)->key, key) != 0 && cur->next != NULL) {
        cur = cur->next;
    }
    if (cur == NULL) {
        return 0;
    }

    return ((struct _hashmap_item*)cur->data)->value;
}

bool hashmap_exists(hashmap* map, const char* key)
{
    hash_t h = hash_str(key);

    size_t idx = h % bucket_size(&map->items);
    llist* item = (llist*)bucket_get_ref(&map->items, idx);

    llist_node* cur = item->head;
    while (key_cmp(((struct _hashmap_item*)cur->data)->key, key) != 0 && cur->next != NULL) {
        cur = cur->next;
    }
    if (cur == NULL) {
        return false;
    }
    return true;
}

hash_t hash_str(const char* str)
{
    hash_t hash = 5381;
    int c;
    while (c = *str++) {
        hash = ((hash << 5) + hash) + c;
    }
    return hash;
}

void hashmap_foreach(hashmap* map, void(cb)(void*))
{
    for (int i = 0; i < bucket_size(&map->items); i++) {
        if (!bucket_full_at(&map->items, i))
            continue;

        llist* ref = (llist*)bucket_get(&map->items, i);
        llist_node* cur = ref->head;
        while (cur != NULL) {
            cb(((struct _hashmap_item*)cur->data)->value);
            cur = cur->next;
        }
    }
}

size_t hashmap_item_count(hashmap* map)
{
    return map->item_count;
}
