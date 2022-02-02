#pragma once

#include "../base/allocator.h"

#if !defined(vector_defaultcapacity)
    #define vector_defaultcapacity 1024
#endif

#define vector_null (vector){.size = 0, .capacity = 0, .data = NULL}

#define vector_newfull(T, n, a) _vector_newfull(sizeof(T), n, a)
#define vector_new(T) vector_newfull(T, vector_defaultcapacity, default_allocator);
#define vector_get(v, T, idx) ((T*)(v).data)[idx]
#define vector_set(v, T, idx, ...) ((T*)(v).data)[idx] = __VA_ARGS__
#define vector_append(v, T, ...) ((T*)_vector_checkcapacity((v)))[(v)->size++] = __VA_ARGS__
#define vector_foreach(i, v) for(i = 0; i < (v).size; i++)

typedef struct vector {
    const allocator* allocator;
    size_t size;
    size_t capacity;
    size_t elementsize;
    void* data;
} vector;

void vector_delete(vector* v);

static inline void vector_clear(vector* v) { v->size = 0; }
static inline void* vector_ptr(vector v) { return v.data; }
static inline size_t vector_size(vector v) { return v.size; }
static inline size_t vector_capacity(vector v) { return v.capacity; }
static inline bool vector_isempty(vector v) { return !v.size || !v.data; }
static inline bool vector_isnull(vector v) { return !v.data; }

vector _vector_newfull(size_t elementsize, size_t capacity, const allocator* a);
void* _vector_checkcapacity(vector* v);
