#pragma once

#include "../base/allocator.h"

#if !defined(dict_initialsize)
    #define dict_initialsize 1031 // Prime number
#endif

#define dict_null (dict){.buckets = NULL, .size = 0, .capacity = 0}

#define dict_nodeitem(K) \
    K key; \
    dict_node _dict

#define _dict_getequal(d, k) (d)->equal ? (d)->equal : \
    _Generic((k), \
        int8_t: _dict_equalinteger8, int16_t: _dict_equalinteger16, int32_t: _dict_equalinteger32, int64_t: _dict_equalinteger64, \
        uint8_t: _dict_equalinteger8, uint16_t: _dict_equalinteger16, uint32_t: _dict_equalinteger32, uint64_t: _dict_equalinteger64, \
        char*: _dict_equalstring, const char*: _dict_equalstring)

#define _dict_gethash(d, k) (d)->hash ? (d)->hash :\
    _Generic((k), \
        int8_t: _dict_hashinteger8, int16_t: _dict_hashinteger16, int32_t: _dict_hashinteger32, int64_t: _dict_hashinteger64, \
        uint8_t: _dict_hashinteger8, uint16_t: _dict_hashinteger16, uint32_t: _dict_hashinteger32, uint64_t: _dict_hashinteger64, \
        char*: _dict_hashstring, const char*: _dict_hashstring)

#define dict_new_full(T, a, e, h, f) (dict){ \
    .allocator = a, \
    .size = 0, \
    .capacity = 0, \
    .offsethint = offsetof(T, _dict), \
    .keyoffsethint = (int)offsetof(T, key) - (int)offsetof(T, _dict), \
    .buckets = NULL, \
    .equal = e, \
    .hash = h, \
    .free = f \
    }

#define dict_new(T) dict_new_full(T, default_allocator, NULL, NULL, NULL)
#define dict_entry(T, n) containerof(n, T, _dict)

#define dict_get(d, T, ...) \
    _dict_getnode(d, \
                 (const dict_node*)&((T){.key = __VA_ARGS__}._dict), \
                 _dict_gethash(&d, __VA_ARGS__), \
                 _dict_getequal(&d, __VA_ARGS__))

#define dict_set(d, T, ...) { \
    T* _gensym(n) = allocator_alloctype((d)->allocator, T); \
    allocator_copy(_gensym(n), T, __VA_ARGS__); \
    _dict_setnode((d), &_gensym(n)->_dict, _dict_gethash(d, _gensym(n)->key), _dict_getequal(d, _gensym(n)->key)); \
}

#define dict_foreach(n, d) \
    if(d.buckets) \
        for(size_t _gensym(i) = 0; _gensym(i) < d.capacity; _gensym(i)++) \
            for(n = d.buckets[_gensym(i)].next; n; n = n->next)

struct dict;
struct dict_node;

typedef bool (*_dict_equal_cb)(struct dict, const struct dict_node*, const struct dict_node*);
typedef size_t (*_dict_hash_cb)(struct dict, const struct dict_node*);

typedef struct dict_node {
    struct dict_node* next;
} dict_node;

typedef struct dict {
    const allocator* allocator;
    size_t size;
    size_t capacity;
    int offsethint;
    int keyoffsethint;
    dict_node* buckets;
    _dict_equal_cb equal;
    _dict_hash_cb hash;
    void (*free)(struct dict, const dict_node*);
} dict;

void dict_delete(dict* d);

static inline size_t dict_size(dict d) { return d.size; }
static inline size_t dict_capacity(dict d) { return d.capacity; }
static inline bool dict_isempty(dict d) { return !d.size || !d.buckets; }
static inline bool dict_isnull(dict d) { return !d.buckets; }
static inline float dict_loadfactor(dict d) { return (float)d.size / (float)d.capacity; }

void _dict_setnode(dict* d, dict_node* n, _dict_hash_cb hash, _dict_equal_cb equal);
const dict_node* _dict_getnode(dict d, const dict_node* n, _dict_hash_cb hash, _dict_equal_cb equal);
bool _dict_equalinteger8(dict d, const dict_node* n1, const dict_node* n2);
bool _dict_equalinteger16(dict d, const dict_node* n1, const dict_node* n2);
bool _dict_equalinteger32(dict d, const dict_node* n1, const dict_node* n2);
bool _dict_equalinteger64(dict d, const dict_node* n1, const dict_node* n2);
bool _dict_equalstring(dict d, const dict_node* n1, const dict_node* n2);
size_t _dict_hashinteger8(dict d, const dict_node* n);
size_t _dict_hashinteger16(dict d, const dict_node* n);
size_t _dict_hashinteger32(dict d, const dict_node* n);
size_t _dict_hashinteger64(dict d, const dict_node* n);
size_t _dict_hashstring(dict d, const dict_node* n);
