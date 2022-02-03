#pragma once

#include "common.h"

#define allocator_declare(allocfn, resizefn, freefn) &(allocator){.alloc = allocfn, .resize = resizefn, .free = freefn}
#define allocator_alloctypezero(a, T) (T*)allocator_alloczero((a), sizeof(T))
#define allocator_alloctype(a, T) (T*)(a)->alloc(sizeof(T))
#define allocator_copy(ptr, T, ...) (*(T*)(ptr)) = (T)__VA_ARGS__

typedef struct allocator {
    void* (*alloc)(size_t size);
    void* (*resize)(void* ptr, size_t oldsize, size_t newsize);
    void  (*free)(void* ptr);
} allocator;

extern const allocator* default_allocator;

void* allocator_alloczero(const allocator* a, size_t size);
void* allocator_resizezero(const allocator* a, void* ptr, size_t oldsize, size_t newsize);
