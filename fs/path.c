#include "path.h"
#include <string.h>

#if defined(clib_platform_windows)
    #if !defined(WIN32_LEAN_AND_MEAN)
        #define WIN32_LEAN_AND_MEAN
    #endif
    #include <windows.h>
#elif defined(clib_platform_posix)
    #include <sys/types.h>
    #include <sys/stat.h>
    #include <unistd.h>
#else
    static_assert(false, "path is not supported");
#endif

path path_litfull(const char* s, const allocator* a) { return s ? (path) {.str = string_litfull(s, a)} : path_null; }
path path_newfull(string s, const allocator* a) { return (path) {.str = string_dupfull(s, a)}; }
void path_delete(path* path) { if(path) string_delete(&path->str); }

stringview path_path(path p)
{
    int idx = string_lastindex(p.str, path_separator);
    return (idx != -1) ? string_sub(p.str, 0, idx) : stringview_null;
}

stringview path_name(path p)
{
    int idx = string_lastindex(p.str, path_separator);
    return (idx != -1) ? string_sub(p.str, idx + 1, p.str.size) : stringview_null;
}

stringview path_stem(path p)
{
    int startidx = string_lastindex(p.str, path_separator);
    int endidx = string_lastindex(p.str, '.');
    if((startidx == -1) || (endidx == -1) || (startidx >= endidx)) return stringview_null;
    return string_sub(p.str, startidx + 1, endidx);
}

stringview path_ext(path p)
{
    int idx = string_lastindex(p.str, '.');
    return (idx != -1) ? string_sub(p.str, idx, p.str.size) : stringview_null;
}

bool path_issymlink(path p)
{
#if defined(clib_platform_windows)
    DWORD attributes = GetFileAttributesA(string_ptr(p.data));
    return (attributes != INVALID_FILE_ATTRIBUTES) && (attributes & FILE_ATTRIBUTE_REPARSE_POINT);
#else
    struct stat buf;
    return !lstat(string_ptr(p.str), &buf) && S_ISLNK(buf.st_mode);
#endif
}

bool path_isfile(path p)
{
#if defined(clib_platform_windows)
    DWORD attributes = GetFileAttributesA(string_ptr(p.data));
    return (attributes != INVALID_FILE_ATTRIBUTES) && !(attributes & FILE_ATTRIBUTE_DIRECTORY);
#else
    struct stat buf;
    return !lstat(string_ptr(p.str), &buf) && S_ISREG(buf.st_mode);
#endif
}

bool path_isdir(path p)
{
#if defined(clib_platform_windows)
    DWORD attributes = GetFileAttributesA(string_ptr(p.data));
    return (attributes != INVALID_FILE_ATTRIBUTES) && (attributes & FILE_ATTRIBUTE_DIRECTORY);
#else
    struct stat buf;
    return !lstat(string_ptr(p.str), &buf) && S_ISDIR(buf.st_mode);
#endif
}

void _path_appendchar(path* p, char ch)
{
    if(ch == path_separator) return;

    if(!string_endswith(p->str, path_separator))
        _path_concatchar(p, path_separator);

    _path_concatchar(p, ch);
}

void _path_appendcstring(path* p, const char* s)
{
    if(!s || !*s) return;

    if(!string_endswith(p->str, path_separator))
        _path_concatchar(p, path_separator);

    _path_concatcstring(p, s);
}

void _path_appendstring(path* p, string s)
{
    if(string_isempty(s)) return;

    if(!string_endswith(p->str, path_separator))
        _path_concatchar(p, path_separator);

    _path_concatstring(p, s);
}

void _path_appendstringview(path* p, stringview sv)
{
    if(stringview_isempty(sv)) return;

    if(!string_endswith(p->str, path_separator))
        _path_concatchar(p, path_separator);

    _path_concatstringview(p, sv);
}

void _path_appendpath(path* p, path pth) { if(!path_isempty(pth)) _path_appendstring(p, pth.str); }
void _path_concatchar(path* p, char ch) { _string_appendchar(&p->str, ch); }
void _path_concatcstring(path* p, const char* s) { if(s && *s) _string_appendcstring(&p->str, s); }
void _path_concatstring(path* p, string s) { if(!string_isempty(s)) _string_appendstring(&p->str, s); }
void _path_concatstringview(path* p, stringview sv) { if(!stringview_isempty(sv)) _string_appendstringview(&p->str, sv); }
void _path_concatpath(path* p, path pth) { if(!path_isempty(pth)) _string_appendstring(&p->str, pth.str); }
void _path_copycstring(path* p, const char* cstr) { _string_copycstring(&p->str, cstr, strlen(cstr)); }
void _path_copystring(path* p, string s) { _string_copystring(&p->str, s, s.size); }
void _path_copystringview(path* p, stringview sv) { _string_copystringview(&p->str, sv, sv.size); }
void _path_copypath(path* p, path pth) { if(!path_isempty(pth)) _path_copystring(p, pth.str); }
