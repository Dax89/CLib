#include "string.h"
#include "../base/base.h"
#include "../base/private/string.h"
#include "stringview.h"
#include <string.h>

static void _string_checkcapacity(string* s, size_t newsize)
{
    if(!s || !s->data) return;

    size_t size = s->size + newsize;
    if(size < s->capacity) return;

    if(!s->capacity) s->capacity = size;

    while(size >= s->capacity)
        s->capacity <<= 1;

    s->data = s->allocator->resize(s->data, s->size + 1, s->capacity);
}

static void _string_convertinteger(string* s, uintmax_t num, size_t base)
{
    while(num)
    {
        string_append(s, digits_ascii_lowercase[num % base]);
        num /= base;
    }
}

void _string_appendchar(string* s, char c)
{
    _string_checkcapacity(s, 1);
    s->data[s->size++] = c;
    s->data[s->size] = '\0';
}

void _string_appendcstring(string* s, const char* cstr)
{
    if(!cstr || !(*cstr)) return;

    size_t size = strlen(cstr);
    _string_checkcapacity(s, size);

    memcpy(s->data + s->size, cstr, size);
    s->size += size;
    s->data[s->size] = '\0';
}

void _string_appendstring(string* s, string str)
{
    if(string_isempty(str)) return;

    _string_checkcapacity(s, str.size);

    memcpy(s->data + s->size, str.data, str.size);
    s->size += str.size;
    s->data[s->size] = '\0';
}


void _string_appendstringview(string* s, stringview sv)
{
    if(stringview_isempty(sv)) return;

    _string_checkcapacity(s, sv.size);
    memcpy(s->data + s->size, sv.data, sv.size);
    s->size += sv.size;
    s->data[s->size] = '\0';
}

string string_newfull(size_t size, const allocator* a)
{
    return (string){
        .allocator =  a,
        .data = (char*)allocator_alloczero(a, size + 1),
        .capacity = size,
        .size = 0,
    };
}

string string_litfull(const char* s, const allocator* a)
{
    size_t size = s && *s ? (size_t)strlen(s) : 0;
    if(!size) return string_null;

    string res = {
        .allocator =  a,
        .data = (char*)a->alloc(size + 1),
        .capacity = size + 1,
        .size = size,
    };

    memcpy(res.data, s, size);
    res.data[size] = '\0';
    return res;
}

void string_delete(string* s)
{
    if(s->allocator) s->allocator->free(s->data);
    s->capacity = 0;
    s->size = 0;
    s->data = NULL;
}

bool string_equal(string s1, string s2) { return s1.data && s2.data && !strcmp(s1.data, s2.data); }

void _string_copycstring(string* s, const char* from, size_t size)
{
    size_t sz = strlen(from);
    if(size < sz) sz = size;

    _string_checkcapacity(s, sz);
    strncpy(s->data, from, sz);
    s->size = sz;
    s->data[s->size] = '\0';
}

void _string_copystring(string* s, string from, size_t size) { _string_copycstring(s, from.data, size); }
void _string_copystringview(string* s, stringview from, size_t size) { _string_copycstring(s, from.data, size); }

string string_fromsignedfull(intmax_t num, size_t base, const allocator* a)
{
    int sign = 1;
    if(num < 0) sign = -1;
    num *= sign;

    string s = string_newfull(32, a);
    _string_convertinteger(&s, (uintmax_t)num, base);
    if(sign < 0) string_append(&s, "-");
    string_reverse(&s);
    return s;
}

string string_fromunsignedfull(uintmax_t num, size_t base, const allocator* a)
{
    string s = string_newfull(32, a);
    _string_convertinteger(&s, num, base);
    string_reverse(&s);
    return s;
}

string string_dupfull(string s, const allocator* a)
{
    string ns = string_newfull(s.size, a);
    string_copy(&ns, s, s.size);
    return ns;
}

void string_reverse(string* s)
{
    for(int i = 0, j = s->size - 1; i < j; i++, j--)
    {
        char c = s->data[i];
        s->data[i] = s->data[j];
        s->data[j] = c;
    }
}

string string_reversedfull(string s, const allocator* a)
{
    string ns = string_dupfull(s, a);
    string_reverse(&ns);
    return ns;
}

string string_subbedfull(string s, int start, int end, const allocator* a)
{
    if(start < 0) start += s.size + 1;
    if(end < 0) end += s.size + 1;

    if((start < 0) || (start > (int)s.size) || (end < start) || (end < 0) || (end > (int)s.size))
        return string_null;

    size_t len = (size_t)(end - start);
    string ns = string_newfull(len, a);
    string_copy(&ns, s, len);
    return ns;
}

stringview string_view(string s) { return stringview_newsize(s.data, s.size); }

stringview string_sub(string s, int start, int end)
{
    if(start < 0) start += s.size + 1;
    if(end < 0) end += s.size + 1;
    if(end > (int)s.size) end = (int)s.size;

    if((start < 0) || (start > (int)s.size) || (end < start) || (end < 0))
        return stringview_null;

    return stringview_newsize(s.data + start, (size_t)(end - start));
}

int _string_indexchar(string s, char ch) { return _cstring_indexchar(s.data, s.size, ch); }
int _string_indexcstring(string s, const char* cstr) { return _cstring_index(s.data, s.size, cstr, strlen(cstr)); }
int _string_indexstring(string s, string str) { return _cstring_index(s.data, s.size, str.data, str.size); }
int _string_indexstringview(string s, stringview str) { return _cstring_index(s.data, s.size, str.data, str.size);  }
int _string_lastindexchar(string s, char ch) { return _cstring_lastindexchar(s.data, s.size, ch); }
int _string_lastindexcstring(string s, const char* cstr) { return _cstring_lastindex(s.data, s.size, cstr, strlen(cstr)); }
int _string_lastindexstring(string s, string str) { return _cstring_lastindex(s.data, s.size, str.data, str.size); }
int _string_lastindexstringview(string s, stringview str) { return _cstring_lastindex(s.data, s.size, str.data, str.size); }
bool _string_startswithchar(string s, char ch) { return _cstring_startswithchar(s.data, s.size, ch); }
bool _string_startswithcstring(string s, const char* cstr) { return _cstring_startswith(s.data, s.size, cstr, strlen(cstr)); }
bool _string_startswithstring(string s, string str) { return _cstring_startswith(s.data, s.size, str.data, str.size); }
bool _string_startswithstringview(string s, stringview sv) { return _cstring_startswith(s.data, s.size, sv.data, sv.size); }
bool _string_endswithchar(string s, char ch) { return _cstring_endswithchar(s.data, s.size, ch); }
bool _string_endswithcstring(string s, const char* cstr) { return _cstring_endswith(s.data, s.size, cstr, strlen(cstr)); }
bool _string_endswithstring(string s, string str) { return _cstring_endswith(s.data, s.size, str.data, str.size); }
bool _string_endswithstringview(string s, stringview sv) { return _cstring_endswith(s.data, s.size, sv.data, sv.size); }
