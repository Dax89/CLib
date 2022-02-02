#include "hash.h"
#include <string.h>

size_t hash_integer(size_t val)
{
    // Robert Jenkins' 32 bit Mix Function
    val += (val << 12);
    val ^= (val >> 22);
    val += (val << 4);
    val ^= (val >> 9);
    val += (val << 10);
    val ^= (val >> 2);
    val += (val << 7);
    val ^= (val >> 12);

    // Knuth's Multiplicative Method
    val = (val >> 3) * 2654435761;
    return val;
}

// https://github.com/abrandoned/murmur2/blob/master/MurmurHash2.c
size_t hash_stringsize(const char* s, size_t size)
{
    /* 'M' and 'R' are mixing constants generated offline.
     They're not really 'magic', they just happen to work well.  */

    const uint32_t M = 0x5bd1e995;
    const int R = 24;

    uint32_t h = (uint32_t)(0 ^ size); /* Initialize the hash to a 'random' value */

    /* Mix 4 bytes at a time into the hash */
    const unsigned char* data = (const unsigned char*)s;

    while(size >= 4)
    {
        uint32_t k = *(uint32_t*)data;

        k *= M;
        k ^= k >> R;
        k *= M;

        h *= M;
        h ^= k;

        data += 4;
        size -= 4;
    }

    /* Handle the last few bytes of the input array  */

    switch(size)
    {
        case 3: h ^= data[2] << 16; /* fall through */
        case 2: h ^= data[1] << 8; /* fall through */
        case 1: h ^= data[0]; h *= M;
    }

    /* Do a few final mixes of the hash to ensure the last few
     bytes are well-incorporated.  */

    h ^= h >> 13;
    h *= M;
    h ^= h >> 15;

    return h;
}

size_t hash_string(const char* s) { return hash_stringsize(s, strlen(s)); }
