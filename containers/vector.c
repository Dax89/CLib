#include "vector.h"

void* _vector_checkcapacity(vector* v)
{
    if(v->data && (v->size + v->elementsize < v->capacity))
        return v->data;

    if(!v->capacity) v->capacity = vector_defaultcapacity * v->elementsize;
    else v->capacity <<= 1;

    v->data = v->allocator->resize(v->data, v->size, v->capacity);
    return v->data;
}

void vector_delete(vector* v)
{
    v->allocator->free(v->data);
    v->data = NULL;
    v->capacity = 0;
    v->size = 0;
}

vector _vector_newfull(size_t elementsize, size_t capacity, const allocator* a)
{
    return (vector){
        .allocator = a,
        .capacity = capacity,
        .elementsize = elementsize,
        .size = 0,
        .data = a->alloc(elementsize * capacity)
    };
}
