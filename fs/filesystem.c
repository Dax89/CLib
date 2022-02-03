#include "filesystem.h"

#if defined(clib_platform_windows)
    #include <errno.h>
    #include <direct.h>
#elif defined(clib_platform_posix)
    #include <errno.h>
    #include <sys/types.h>
    #include <sys/stat.h>
    #include <unistd.h>
    #include <dirent.h>
    #include <string.h>
#else
    static_assert(false, "Filesystem operations not supported");
#endif

static bool _filesystem_mkdir(const char* dir)
{
    int r = 0;

#if defined(clib_platform_windows)
    r = _mkdir(dir);
#elif defined(clib_platform_posix)
    r = mkdir(dir, 0700);
#endif

    return !r || ((r == -1) && errno == EEXIST);
}

static bool _filesystem_mkpath(const char* dir)
{
    path fullp = path_lit(dir), p = path_new();
    bool res = false;

    defer(path_delete(&p), path_delete(&fullp))
    {
        stringview sv; path_foreach(sv, fullp)
        {
            path_append(&p, sv);
            res = _filesystem_mkdir(path_ptr(p));
            if(!res) break;
        }
    }

    return res;
}

filesystem _filesystem_new(const char* dir, const allocator* allocator)
{
    return (filesystem){
        .handle = (uintptr_t)opendir(dir),
        .wd = path_litfull(dir, allocator)};
}

void _filesystem_delete(filesystem* fs)
{
    if(!fs || !fs->handle) return;

    closedir((DIR*)fs->handle);
    path_delete(&fs->it);
    path_delete(&fs->wd);
    fs->handle = 0;
}

bool _filesystem_next(filesystem* fs)
{
    DIR* dir = (DIR*)fs->handle;
    struct dirent* dp = NULL;

    do dp = readdir(dir);
    while(dp && (!strcmp(dp->d_name, ".") || !strcmp(dp->d_name, "..")));

    if(dp)
    {
        if(path_isnull(fs->it)) fs->it = path_dup(fs->wd);
        else path_copy(&fs->it, fs->wd);
        path_append(&fs->it, dp->d_name);
    }
    else
        path_clear(&fs->it);

    return dp;
}

bool _filesystem_mkdircstring(const char* cstr) { return _filesystem_mkdir(cstr); }
bool _filesystem_mkdirstring(string s) { return _filesystem_mkdir(s.data); }
bool _filesystem_mkdirpath(path p) { return _filesystem_mkdir(p.str.data); }
bool _filesystem_mkpathcstring(const char* cstr) { return _filesystem_mkpath(cstr); }
bool _filesystem_mkpathstring(string s) { return _filesystem_mkpath(s.data); }
bool _filesystem_mkpathpath(path p) { return _filesystem_mkpath(p.str.data); }
