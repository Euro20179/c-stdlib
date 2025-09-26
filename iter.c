#include "iter.h"
#include <stdlib.h>

void* next(struct iterable_t* iter)
{
    void* res = iter->nextfn(iter->self);
    return res;
}

struct iterable_t* create_iter(void* obj, void (*iterstartfn)(void* self, struct iterable_t* i), void* (*nextfn)(void* self))
{
    struct iterable_t* i = (struct iterable_t*)malloc(sizeof(struct iterable_t));
    i->self = obj;
    i->nextfn = nextfn;
    iterstartfn(obj, i);
    return i;
}

void iter_done(struct iterable_t* iter)
{
    free(iter);
}
