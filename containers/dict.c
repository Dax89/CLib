#include "dict.h"
#include "../base/hash.h"
#include <string.h>

#define _dict_defaultloadfactor 0.75

static bool _dict_isprime(size_t number)
{
    if(number == 2 || number == 3) return true;
    if(number % 2 == 0 || number % 3 == 0) return false;

    size_t divisor = 6;

    while((divisor * divisor - 2 * divisor + 1) <= number)
    {
        if(number % (divisor - 1) == 0) return false;
        if(number % (divisor + 1) == 0) return false;
        divisor += 6;
    }

    return true;
}

static size_t _dict_nextprime(size_t number)
{
    while(!_dict_isprime(number)) number++;
    return number;
}

static inline void _dict_freenode(dict d, dict_node* n)
{
    if(d.free) d.free(d, n);
    d.allocator->free(containerbase(n, void, d.offsethint));
}

static bool _dict_insertnode(dict* d, dict_node* n, _dict_hash_cb hash, _dict_equal_cb equal)
{
    size_t idx = hash(*d, n) % d->capacity;
    n->next = NULL;

    if(d->buckets[idx].next)
    {
        dict_node **ncur = &d->buckets[idx].next, **nprev = NULL;

        while(*ncur)
        {
            if(equal && equal(*d, *ncur, n))
            {
                if(nprev) (*nprev)->next = n;
                n->next = (*ncur)->next;
                _dict_freenode(*d, *ncur);
                *ncur = n;
                return false;
            }

            nprev = ncur;
            ncur = &(*ncur)->next;
        }

        (*ncur)->next = n;
    }
    else
        d->buckets[idx].next = n;

    return true;
}

static void _dict_rehash(dict* d, _dict_hash_cb hash)
{
    size_t oldcapacity = d->capacity;
    dict_node *oldbuckets = d->buckets;

    d->capacity = _dict_nextprime(d->capacity << 1);
    d->buckets = allocator_alloczero(d->allocator, d->capacity * sizeof(dict_node));

    for(size_t i = 0; i < oldcapacity; i++)
    {
        dict_node* n = oldbuckets[i].next;

        while(n)
        {
            dict_node* next = n->next;
            _dict_insertnode(d, n, hash, NULL);
            n = next;
        }
    }

    d->allocator->free(oldbuckets);
}

static void _dict_checkcapacity(dict* d, _dict_hash_cb hash)
{
    if(!d->capacity)
    {
        d->capacity = dict_initialsize;
        d->buckets = allocator_resizezero(d->allocator, d->buckets, d->size * sizeof(dict_node), d->capacity * sizeof(dict_node));
        return;
    }

    if(dict_loadfactor(*d) <= _dict_defaultloadfactor) return;
    _dict_rehash(d, hash);
}

static bool _dict_equalinteger(size_t k1, size_t k2) { return k1 == k2; }

const dict_node* _dict_getnode(dict d, const dict_node* n, _dict_hash_cb hash, _dict_equal_cb equal)
{
    if(!d.buckets || !d.capacity) return NULL;

    size_t idx = hash(d, n) % d.capacity;
    dict_node* ncur = d.buckets[idx].next;

    while(ncur)
    {
        if(equal(d, ncur, n)) return ncur;
        ncur = ncur->next;
    }

    return NULL;
}

void _dict_setnode(dict* d, dict_node* n, _dict_hash_cb hash, _dict_equal_cb equal)
{
    _dict_checkcapacity(d, hash);

    if(_dict_insertnode(d, n, hash, equal))
        d->size++;
}

bool _dict_equalinteger8(dict d, const dict_node* n1, const dict_node* n2)
{
    return _dict_equalinteger((size_t)*containerptr(n1, uint8_t, d.keyoffsethint),
                              (size_t)*containerptr(n2, uint8_t, d.keyoffsethint));
}

bool _dict_equalinteger16(dict d, const dict_node* n1, const dict_node* n2)
{
    return _dict_equalinteger((size_t)*containerptr(n1, uint16_t, d.keyoffsethint),
                              (size_t)*containerptr(n2, uint16_t, d.keyoffsethint));
}

bool _dict_equalinteger32(dict d, const dict_node* n1, const dict_node* n2)
{
    return _dict_equalinteger((size_t)*containerptr(n1, uint32_t, d.keyoffsethint),
                              (size_t)*containerptr(n2, uint32_t, d.keyoffsethint));
}

bool _dict_equalinteger64(dict d, const dict_node* n1, const dict_node* n2)
{
    return _dict_equalinteger((size_t)*containerptr(n1, uint64_t, d.keyoffsethint),
                              (size_t)*containerptr(n2, uint64_t, d.keyoffsethint));
}

bool _dict_equalstring(dict d, const dict_node* n1, const dict_node* n2)
{
    return !strcmp(*containerptr(n1, const char*, d.keyoffsethint),
                   *containerptr(n2, const char*, d.keyoffsethint));
}

size_t _dict_hashinteger8(dict d, const dict_node* n)  { return hash_integer((size_t)*containerptr(n, uint8_t, d.keyoffsethint));  }
size_t _dict_hashinteger16(dict d, const dict_node* n) { return hash_integer((size_t)*containerptr(n, uint16_t, d.keyoffsethint)); }
size_t _dict_hashinteger32(dict d, const dict_node* n) { return hash_integer((size_t)*containerptr(n, uint32_t, d.keyoffsethint)); }
size_t _dict_hashinteger64(dict d, const dict_node* n) { return hash_integer((size_t)*containerptr(n, uint64_t, d.keyoffsethint)); }
size_t _dict_hashstring(dict d, const dict_node* n) { return hash_string(*containerptr(n, const char*, d.keyoffsethint)); }

void dict_delete(dict* d)
{
    for(size_t i = 0; i < d->capacity; i++)
    {
        dict_node* n = d->buckets[i].next;

        while(n)
        {
            dict_node* next = n->next;
            _dict_freenode(*d, n);
            n = next;
        }
    }

    d->allocator->free(d->buckets);
    d->size = d->capacity = 0;
    d->buckets = NULL;
}
