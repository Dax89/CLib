#include "filesystem.h"

#if defined(clib_platform_posix)
    #include <dirent.h>
    #include <string.h>
#endif

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
