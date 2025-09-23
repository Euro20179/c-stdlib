#pragma once
#include <stddef.h>
#include <stdint.h>

typedef struct llist_node llist_node;

struct llist_node{
    void* data;
    struct llist_node* next;
};

llist_node* llist_node_create(void* data);

void llist_node_destroy(llist_node*);


typedef struct llist{
    struct llist_node* head;
    struct llist_node* last;
    size_t len;
} llist;

#define llist(ty) struct llist

void llist_new(llist*);

void llist_del(llist*);

void llist_append(llist*, void* data);

void llist_remove(llist*, size_t idx);

void llist_clear(llist*);

///returns the index of data
///returns -1 if not found
///
///NOTE: the comparison function simply checks if both pointers point to the same place
///this means that if the same pointer is interpreted as 2 different ways be the caller intentionally
///this will return true, despite the programmer treating the same pointer in 2 different ways
///
///eg:
///int k = 3
///double* z = &k
///llist_find would say z == k because z == &k
int64_t llist_find(llist*, void* data);

///gets the item at the nth index (head is the 0th index)
///returns NULL if out of bounds
void* llist_at(llist*, size_t n);
