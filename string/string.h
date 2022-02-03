#pragma once

#include "../base/allocator.h"
#include "stringview.h"

#if !defined(string_defaultcapacity)
    #define string_defaultcapacity 32
#endif

#define string_null (string){.data = NULL, .capacity = 0, .size = 0}

#define string_copy(s, from, size) \
    _Generic((from), \
        string: _string_copystring, \
        const char*: _string_copycstring, \
        char*: _string_copycstring)((s), (from), (size))

#define string_append(s, a) \
    _Generic((a), \
        string: _string_appendstring, \
        stringview: _string_appendstringview, \
        const char*: _string_appendcstring, \
        char*: _string_appendcstring, \
        char: _string_appendchar, \
        int: _string_appendchar)((s), (a))

#define string_index(s, arg) \
    _Generic((arg), \
        string: _string_indexstring, \
        stringview: _string_indexstringview, \
        const char*: _string_indexcstring, \
        char*: _string_indexcstring, \
        char: _string_indexchar, \
        int: _string_indexchar)((s), (arg))

#define string_startswith(s, arg) \
    _Generic((arg), \
        string: _string_startswithstring, \
        stringview: _string_startswithstringview, \
        const char*: _string_startswithcstring, \
        char*: _string_startswithcstring, \
        char: _string_startswithchar, \
        int: _string_startswithchar)((s), (arg))

#define string_endswith(s, arg) \
    _Generic((arg), \
        string: _string_endswithstring, \
        stringview: _string_endswithstringview, \
        const char*: _string_endswithcstring, \
        char*: _string_endswithcstring, \
        char: _string_endswithchar, \
        int: _string_endswithchar)((s), (arg))

#define string_lastindex(s, arg) \
    _Generic((arg), \
        string: _string_lastindexstring, \
        stringview: _string_lastindexstringview, \
        const char*: _string_lastindexcstring, \
        char*: _string_lastindexcstring, \
        char: _string_lastindexchar, \
        int: _string_lastindexchar)((s), (arg))


#define string_spliteach(iter, s, sep) \
    iter = stringview_null; \
    for(bool _gensym(ok) = _string_splitnext(&iter, s, sep); \
        _gensym(ok); \
        _gensym(ok) = _string_splitnext(&iter, s, sep)) \
        if(!stringview_isempty(iter))

#define string_foreach(c, s) \
    if(!string_isempty((s))) c = (s).data[0]; \
    for(size_t _gensym(i) = 0; \
        (s).data && (_gensym(i) < (s).size); \
        c = (s).data[++_gensym(i)])

typedef struct string {
    const allocator* allocator;
    char* data;
    size_t capacity;
    size_t size;
} string;

string string_newfull(size_t size, const allocator* a);
string string_litfull(const char* s, const allocator* a);
string string_fromsignedfull(intmax_t num, size_t base, const allocator* a);
string string_fromunsignedfull(uintmax_t num, size_t base, const allocator* a);
string string_dupfull(string s, const allocator* a);
string string_reversedfull(string s, const allocator* a);
string string_subbedfull(string s, int start, int end, const allocator* a);
stringview string_view(string s);
stringview string_sub(string s, int start, int end);
bool string_equal(string s1, string s2);
void string_reverse(string* s);
void string_delete(string* s);

static inline string string_newsize(size_t size) { return string_newfull(size, default_allocator); }
static inline string string_new() { return string_newsize(string_defaultcapacity); }
static inline string string_lit(const char* s) { return string_litfull(s, default_allocator); }
static inline string string_dup(string s) { return string_dupfull(s, default_allocator); }
static inline string string_subbed(string s, int start, int end) { return string_subbedfull(s, start, end, default_allocator); }
static inline string string_fromsigned(intmax_t num, size_t base) { return string_fromsignedfull(num, base, default_allocator); }
static inline string string_fromunsigned(uintmax_t num, size_t base) { return string_fromunsignedfull(num, base, default_allocator); }
static inline const char* string_ptr(string s) { return s.data; }
static inline size_t string_size(string s) { return s.size; }
static inline size_t string_capacity(string s) { return s.capacity; }
static inline void string_clear(string s) { if(s.data && s.capacity) s.data[0] = '\0'; s.size = 0; }
static inline char string_at(string s, size_t pos) { return s.data && pos < s.size ? s.data[pos] : '\0'; }
static inline bool string_isempty(string s) { return !s.data || !s.size; }
static inline bool string_isnull(string s) { return !s.data; }

bool _string_splitnext(stringview* sv, string s, char sep);
bool _string_startswithchar(string s, char ch);
bool _string_startswithcstring(string s, const char* cstr);
bool _string_startswithstring(string s, string str);
bool _string_startswithstringview(string s, struct stringview sv);
bool _string_endswithchar(string s, char ch);
bool _string_endswithcstring(string s, const char* cstr);
bool _string_endswithstring(string s, string str);
bool _string_endswithstringview(string s, struct stringview sv);
int _string_indexcstring(string s, const char* cstr);
int _string_indexstring(string s, string str);
int _string_indexstringview(string s, struct stringview str);
int _string_lastindexchar(string s, char ch);
int _string_lastindexcstring(string s, const char* cstr);
int _string_lastindexstring(string s, string str);
int _string_lastindexstringview(string s, struct stringview str);
void _string_copycstring(string* s, const char* from, size_t size);
void _string_copystring(string* s, string from, size_t size);
void _string_copystringview(string* s, stringview from, size_t size);
void _string_appendchar(string* s, char c);
void _string_appendcstring(string* s, const char* cstr);
void _string_appendstring(string* s, string str);
void _string_appendstringview(string* s, stringview sv);
