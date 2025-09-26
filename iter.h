#pragma once
#include <stddef.h>

/*
                                 = iterables =
This file defines functions that enable types to be iterable.
Types can be iterable if they define:
    + <type>_iter(<type>*, struct iterable_t*)
    + <type>_next(<type>*)
If these functions are defined, the user may call foreach(<TItem>, instance, <type>, { code })
and code will run for each item of type <TItem> in instance of type <type>.

2 magical vars are defined in the foreach macro:
    + cur
        - the current item
    + it
        - the iterable itself
*/

struct iterable_t {
    void* (*nextfn)(void* self);
    void* self;
};

#define is ,

#define iter(ty, obj) create_iter(obj, (void (*)(void*, struct iterable_t*))ty##_iter, (void* (*)(void*))ty##_next)

#define foreach(tyIT, O, T, todo)                   \
    {                                              \
        struct iterable_t* it = iter(T, O); \
        tyIT cur = NULL;                          \
        while ((cur = next(it)) != NULL) {         \
            todo;                                  \
        }                                          \
        iter_done(it);                             \
    }

struct iterable_t* create_iter(void* obj, void (*iterstartfn)(void* self, struct iterable_t* i), void* (*nextfn)(void* self));
void* next(struct iterable_t*);

void iter_done(struct iterable_t*);
