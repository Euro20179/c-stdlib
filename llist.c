#include "llist.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

llist_node* llist_node_create(void* data)
{
    llist_node* n = malloc(sizeof(llist_node));
    n->data = data;
    n->next = NULL;
    return n;
}

void llist_node_destroy(llist_node* node)
{
    free(node);
}

void* llist_node_get(llist_node* n)
{
    return n->data;
}

void llist_node_set(llist_node* n, void* data)
{
    n->data = data;
}

void llist_new(llist* l)
{
    l->head = NULL;
    l->last = NULL;
    l->len = 0;
}

llist* llist_new2()
{
    llist* l = malloc(sizeof(llist));
    l->head = NULL;
    l->last = NULL;
    l->len = 0;
    return l;
}

void llist_append(llist* l, void* data)
{
    l->len++;
    if (l->head == NULL) {
        l->head = llist_node_create(data);
        l->last = l->head;
    } else {
        l->last->next = llist_node_create(data);
        l->last = l->last->next;
    }
}

void llist_del(llist* l)
{
    llist_node* cur = l->head;
    while (cur != NULL) {
        llist_node* next_cur = cur->next;
        llist_node_destroy(cur);
        cur = next_cur;
    }
    l->head = 0;
    l->last = 0;
    l->len = 0;
}

void llist_clear(llist* l)
{
    llist_del(l);
}

void llist_remove(llist* l, size_t idx)
{
    llist_node* cur = l->head;
    size_t cur_idx = 0;
    llist_node* prev = NULL;
    while (cur_idx < idx && cur->next != NULL) {
        prev = cur;
        cur = cur->next;
        cur_idx++;
    }

    // idx out of bounds
    if (cur_idx < idx) {
        return;
    }

    //if prev IS NOT null we are removing a random item
    if (prev != NULL) {
        prev->next = cur->next;
    //if prev IS NULL, we are removing the head
    } else {
        l->head = cur->next;
    }
    llist_node_destroy(cur);

    l->len--;
}

void* llist_at(llist* l, size_t n)
{
    llist_node* cur = l->head;

    size_t i = 0;
    while (i < n) {
        if (cur->next == NULL) {
            return NULL;
        }
        cur = cur->next;
        i++;
    }
    return cur->data;
}

int llist_set_idx(llist* l, size_t idx, void* data)
{
    llist_node* cur = l->head;

    size_t i = 0;

    while (cur != NULL && i < idx - 1) {
        cur = cur->next;
        i++;
    }

    if (i < idx - 1 || cur->next == NULL) {
        return -1;
    }

    llist_node* new = llist_node_create(data);
    new->next = cur->next->next;
    cur->next = new;

    return 0;
}

int64_t llist_find(llist* l, void* data)
{
    llist_node* cur = l->head;
    int64_t idx = -1;
    while (cur != NULL) {
        idx++;
        if (cur->data == data) {
            break;
        }
        cur = cur->next;
    }
    return idx;
}

llist_node* llist_next(llist* l, llist_node* start)
{
    if (start == NULL) {
        start = l->head;
        return start;
    }

    return start->next;
}
