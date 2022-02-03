#pragma once

#include "../base/allocator.h"

#define list_newfull(T, a, f) (list){.allocator = a, .size = 0, .offsethint = offsetof(T, _list), .head = NULL, .tail = NULL, .free = f}
#define list_new(T) list_newfull(T, default_allocator, NULL)
#define list_entry(T, n) containerof(n, T, _list)
#define list_foreach(n, l) for(n = (l).head; (n); (n) = (n)->next)

#define list_prepend(l, T, ...) { \
    T* _gensym(n) = allocator_alloctype((l)->allocator, T); \
    allocator_copy(_gensyn(n), T, __VA_ARGS__); \
    _list_prepend(l, &_gensym(n)._list); \
}

#define list_append(l, T, ...) { \
    T* _gensym(n) = allocator_alloctype((l)->allocator, T); \
    allocator_copy(_gensym(n), T, __VA_ARGS__); \
    _list_append(l, &_gensym(n)->_list); \
}

#define list_nodeitem list_node _list

typedef struct list_node {
    struct list_node* prev;
    struct list_node* next;
} list_node;

typedef struct list {
    const allocator* allocator;
    size_t size;
    int offsethint;
    list_node* head;
    list_node* tail;
    void (*free)(struct list, const list_node*);
} list;

void list_delete(list* l);

static inline size_t list_size(list l) { return l.size; }
static inline bool list_isempty(list l) { return !l.size; }
static inline list_node* list_head(list l) { return l.head; }
static inline list_node* list_tail(list l) { return l.tail; }

void _list_prepend(list* l, list_node* n);
void _list_append(list* l, list_node* n);
