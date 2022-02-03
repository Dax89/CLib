#pragma once

#include "../base/allocator.h"
#include "path.h"

#define filesystem_foreachfull(iter, dir, allocator) \
    filesystem _gensym(fs) = _filesystem_new(dir, allocator); \
    bool _gensym(n) = _filesystem_next(&_gensym(fs)); \
    defer(_filesystem_delete(&_gensym(fs))) \
        if(_gensym(n)) \
            for(iter = _gensym(fs).it; _gensym(n) ; _gensym(n) = _filesystem_next(&_gensym(fs)), iter = _gensym(n) ? _gensym(fs).it : path_null)

#define filesystem_foreach(iter, dir) filesystem_foreachfull(iter, dir, default_allocator)

#define filesystem_mkdir(dir) \
    _Generic((dir), \
             char*: _filesystem_mkdircstring, \
             const char*: _filesystem_mkdircstring, \
             string: _filesystem_mkdirstring, \
             path: _filesystem_mkdirpath)(dir)

#define filesystem_mkpath(dir) \
    _Generic((dir), \
             char*: _filesystem_mkpathcstring, \
             const char*: _filesystem_mkpathcstring, \
             string: _filesystem_mkpathstring, \
             path: _filesystem_mkpathpath)(dir)

typedef struct filesystem {
   uintptr_t handle;
   path wd;
   path it;
} filesystem;

filesystem _filesystem_new(const char* dir, const allocator* a);
void _filesystem_delete(filesystem* fs);
bool _filesystem_next(filesystem* fs);

bool _filesystem_mkdircstring(const char* cstr);
bool _filesystem_mkdirstring(string s);
bool _filesystem_mkdirpath(path p);
bool _filesystem_mkpathcstring(const char* cstr);
bool _filesystem_mkpathstring(string s);
bool _filesystem_mkpathpath(path p);
