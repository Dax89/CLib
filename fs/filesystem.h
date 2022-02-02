#pragma once

#include "../base/allocator.h"
#include "path.h"

#define filesystem_foreach(iter, dir) \
    filesystem _gensym(fs) = _filesystem_new(dir); \
    bool _gensym(n) = _filesystem_next(&_gensym(fs)); \
    defer(_filesystem_delete(&_gensym(fs))) \
        if(_gensym(n)) \
            for(iter = _gensym(fs).it; _gensym(n) ; _gensym(n) = _filesystem_next(&_gensym(fs)), iter = _gensym(n) ? _gensym(fs).it : path_null)

typedef struct filesystem {
   const allocator* allocator;
   uintptr_t handle;
   path wd;
   path it;
} filesystem;

filesystem _filesystem_newfull(const char* dir, const allocator* a);
void _filesystem_delete(filesystem* fs);
bool _filesystem_next(filesystem* fs);

static inline filesystem _filesystem_new(const char* dir) { return _filesystem_newfull(dir, default_allocator); }
