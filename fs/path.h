#pragma once

#include "../string/string.h"
#include "../string/stringview.h"

#if defined(clib_platform_windows)
    #define path_separator '\\'
#else
    #define path_separator '/'
#endif

#define path_null (path){.str = string_null}

#define path_copy(p, arg) \
    _Generic((arg), \
             char*: _path_copycstring, \
             const char*: _path_copycstring, \
             string: _path_copystring, \
             stringview: _path_copystringview, \
             path: _path_copypath)((p), (arg))

#define path_append(p, arg) \
    _Generic((arg), \
             char: _path_appendchar, \
             char*: _path_appendcstring, \
             const char*: _path_appendcstring, \
             string: _path_appendstring, \
             stringview: _path_appendstringview, \
             path: _path_appendpath)((p), (arg))

#define path_concat(p, arg) \
    _Generic((arg), \
             char: _path_concatchar, \
             char*: _path_concatcstring, \
             const char*: _path_concatcstring, \
             string: _path_concatstring, \
             stringview: _path_concatstringview, \
             path: _path_concatpath)((p), (arg))

typedef struct path {
    string str;
} path;

path path_litfull(const char* s, const allocator* a);
path path_newfull(string s, const allocator* a);
stringview path_path(path p);
stringview path_name(path p);
stringview path_stem(path p);
stringview path_ext(path p);
bool path_issymlink(path p);
bool path_isfile(path p);
bool path_isdir(path p);
void path_delete(path* path);

static inline path path_lit(const char* s) { return path_litfull(s, default_allocator); }
static inline path path_new(string s) { return path_newfull(s, default_allocator); }
static inline path path_dup(path p) { return (path){.str = string_dup(p.str)}; }
static inline path path_dupfull(path p, const allocator* a) { return (path){.str = string_dupfull(p.str, a)}; }
static inline stringview path_stringview(path p) { return string_view(p.str); }
static inline bool path_isnull(path p) { return string_isnull(p.str); }
static inline bool path_isempty(path p) { return string_isempty(p.str); }
static inline void path_clear(path* p) { string_clear(p->str); }

void _path_copycstring(path* p, const char* cstr);
void _path_copystring(path* p, string s);
void _path_copystringview(path* p, stringview sv);
void _path_copypath(path* p, path pth);
void _path_appendchar(path* p, char ch);
void _path_appendcstring(path* p, const char* s);
void _path_appendstring(path* p, string s);
void _path_appendstringview(path* p, stringview sv);
void _path_appendpath(path* p, path pth);
void _path_concatchar(path* p, char ch);
void _path_concatcstring(path* p, const char* s);
void _path_concatstring(path* p, string s);
void _path_concatstringview(path* p, stringview sv);
void _path_concatpath(path* p, path pth);
