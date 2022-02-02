#pragma once

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include "platform.h"

#define __concat(a, b) a ## b
#define _concat(a, b) __concat(a, b)
#define _gensym(n) _concat(n, __LINE__)

#define containerof(ptr, type, member) ((type*)((uintptr_t)(ptr) - offsetof(type, member)))
#define containerbase(ptr, type, offset) ((type*)((uintptr_t)(ptr) - offset))
#define containerptr(ptr, type, offset) ((type*)((uintptr_t)(ptr) + offset))
#define scope(start, end) for(int _gensym(i) = (start, 0); !_gensym(i); _gensym(i)++, end)
#define defer(...) for(int _gensym(i) = 0; !_gensym(i); _gensym(i)++, __VA_ARGS__)
