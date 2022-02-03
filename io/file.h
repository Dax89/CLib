#pragma once

#include "../base/allocator.h"

struct string;
struct stringview;
struct vector;

#define file_null (file){.handle = 0, .size = 0}

#define file_open(filepath, mode) \
    _Generic((filepath), \
             char*: _file_opencstring, \
             const char*: _file_opencstring, \
             string: _file_openstring)(filepath, mode)

#define file_write(file, arg) \
    _Generic((arg), \
             char* _file_writecstring, \
             const char* _file_writecstring, \
             string: _file_writestring, \
             stringview: _file_writestringview)

#define file_read(f, size) file_readfull(f, size, default_allocator)
#define file_readstring(f, size) file_readstringfull(f, size, default_allocator)
#define file_readall(f) file_readfull(f, 0, default_allocator)
#define file_readstringall(f) file_readstringfull(f, 0, default_allocator)

typedef struct file {
    uintptr_t handle;
    size_t size;
} file;

enum file_mode {
    file_mode_read   = (1 << 0),
    file_mode_write  = (1 << 1),
    file_mode_append = (1 << 2),

    file_mode_readwrite = file_mode_read | file_mode_write,
};

void file_close(file* f);
struct vector file_readfull(file f, size_t size, const allocator* a);
struct string file_readstringfull(file f, size_t size, const allocator* a);

static inline bool file_isnull(file f) { return !f.handle; }
static inline size_t file_size(file f) { return f.size; }

file _file_opencstring(const char* filepath, size_t mode);
file _file_openstring(struct string s, size_t mode);
size_t _file_writecstring(file* f, const char* s);
size_t _file_writestring(file* f, struct string s);
size_t _file_writestringview(file* f, struct stringview sv);
