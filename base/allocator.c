#include "allocator.h"
#include <stdlib.h>
#include <string.h>

static void* resize(void* ptr, size_t oldsize, size_t newsize)
{
    if(!ptr || !oldsize)
    {
        if(ptr) free(ptr);
        return malloc(newsize);
    }

    void* newptr = memcpy(malloc(newsize), ptr, oldsize);
    free(ptr);
    return newptr;
}

const allocator* default_allocator = allocator_declare(malloc, resize, free);

void* allocator_alloczero(const allocator* a, size_t size) { return memset(a->alloc(size), 0, size); }

void* allocator_resizezero(const allocator* a, void* ptr, size_t oldsize, size_t newsize)
{
    if(!ptr || !oldsize)
    {
        if(ptr) a->free(ptr);
        return allocator_alloczero(a, newsize);
    }

    void* newptr = memcpy(allocator_alloczero(a, newsize), ptr, oldsize);
    free(ptr);
    return newptr;
}
