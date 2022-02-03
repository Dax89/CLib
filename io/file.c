#include "file.h"
#include "../string/string.h"
#include "../string/stringview.h"
#include "../containers/vector.h"
#include <string.h>
#include <stdio.h>

#define file_handle(f) (FILE*)(f)

static size_t _file_write(file* f, const char* s, size_t size)
{
    size_t n = fwrite(s, size, 1, file_handle(f)) * size;
    f->size += n;
    return n;
}

static size_t file_getsize(FILE* f)
{
    long oldpos = ftell(f), size = 0;
    fseek(f, 0, SEEK_END);

    size = ftell(f);
    fseek(f, oldpos, SEEK_SET);
    return size;
}

void file_close(file* f)
{
    if(!f) return;
    if(f->handle) fclose((FILE*)f->handle);
    f->handle = 0;
}

file _file_opencstring(const char* filepath, size_t mode)
{
    char m[5] = {0};
    int idx = 0;

    if(mode & file_mode_append)
    {
        if(mode == file_mode_readwrite)
        {
            m[idx++] = 'a';
            m[idx++] = '+';
        }
        else if(mode & file_mode_write)
            m[idx++] = 'a';
        else
            return file_null;
    }
    else if(mode == file_mode_readwrite)
    {
        m[idx++] = 'w';
        m[idx++] = '+';
    }
    else if(mode & file_mode_read) m[idx++] = 'r';
    else if(mode & file_mode_write) m[idx++] = 'w';
    else return file_null;

#if defined(clib_platform_windows)
    FILE* fp;
    if(fopen_s(&fp, filepath, (const char*)&m)) return file_null;
#else
    FILE* fp = fopen(filepath, (const char*)&m);
#endif

    return (file) {
        .handle = (uintptr_t)fp,
        .size = file_getsize(file_handle(fp))
    };
}

file _file_openstring(string s, size_t mode) { return _file_opencstring(string_ptr(s), mode); }
size_t _file_writecstring(file* f, const char* s) { return s ? _file_write(f, s, strlen(s)) : 0; }
size_t _file_writestring(file* f, struct string s) { return _file_write(f, string_ptr(s), string_size(s)); }
size_t _file_writestringview(file* f, struct stringview sv) { return _file_write(f, stringview_ptr(sv), stringview_size(sv)); }

vector file_readfull(file f, size_t size, const allocator* a)
{
    if(!f.handle) return vector_null;
    if(!size) size = f.size;

    vector v = vector_newfull(uint8_t, size, a);
    v.size = fread(vector_ptr(v), size, 1, file_handle(f.handle)) * size;
    return v;
}

string file_readstringfull(file f, size_t size, const allocator* a)
{
    if(!f.handle) return string_null;
    if(!size) size = f.size;

    string s = string_newfull(size, a);
    s.size = fread((void*)string_ptr(s), size, 1, file_handle(f.handle)) * size;
    return s;
}
