#pragma once

#include "../base/common.h"

#define stringview_null (stringview){.data = NULL, .size = 0}

#define stringview_index(s, arg) \
    _Generic((arg), \
        stringview: _stringview_indexstringview, \
        const char*: _stringview_indexcstring, \
        char*: _stringview_indexcstring, \
        char: _stringview_indexchar, \
        int: _stringview_indexchar)((s), (arg))

#define stringview_lastindex(s, arg) \
    _Generic((arg), \
        stringview: _stringview_lastindexstringview, \
        const char*: _stringview_lastindexcstring, \
        char*: _stringview_lastindexcstring, \
        char: _stringview_lastindexchar, \
        int: _stringview_lastindexchar)((s), (arg))

#define stringview_startswith(s, arg) \
    _Generic((arg), \
        stringview: _stringview_startswithstringview, \
        const char*: _stringview_startswithcstring, \
        char*: _stringview_startswithcstring, \
        char: _stringview_startswithchar, \
        int: _stringview_startswithchar)((s), (arg))

#define stringview_endswith(s, arg) \
    _Generic((arg), \
        stringview: _stringview_endswithstringview, \
        const char*: _stringview_endswithcstring, \
        char*: _stringview_endswithcstring, \
        char: _stringview_endswithchar, \
        int: _stringview_endswithchar)((s), (arg))

typedef struct stringview {
    const char* data;
    size_t size;
} stringview;

stringview stringview_new(const char* cstr);
stringview stringview_slice(stringview sv, int start, int end);
bool stringview_equal(stringview sv1, stringview sv2);

static inline stringview stringview_newsize(const char* s, size_t size) { return (stringview){.data = s, .size = size}; }
static inline const char* stringview_ptr(stringview sv) { return sv.data; }
static inline size_t stringview_size(stringview sv) { return sv.size; }
static inline bool stringview_isempty(stringview sv) { return !sv.data || !sv.size; }
static inline bool stringview_isnull(stringview sv) { return !sv.data; }

bool _stringview_startswithchar(stringview sv, char ch);
bool _stringview_startswithcstring(stringview sv, const char* cstr);
bool _stringview_startswithstringview(stringview sv, stringview strv);
bool _stringview_endswithchar(stringview sv, char ch);
bool _stringview_endswithcstring(stringview sv, const char* cstr);
bool _stringview_endswithstringview(stringview sv, stringview strv);
int _stringview_indexchar(stringview sv, char ch);
int _stringview_indexstringview(stringview sv, stringview strv);
int _stringview_indexcstring(stringview sv, const char* cstr);
int _stringview_lastindexchar(stringview sv, char ch);
int _stringview_lastindexstringview(stringview sv, stringview strv);
int _stringview_lastindexcstring(stringview sv, const char* cstr);
