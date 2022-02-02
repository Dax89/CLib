#include "stringview.h"
#include "../base/private/string.h"
#include <string.h>

stringview _stringview_new(const char* cstr) { return cstr ? stringview_newsize(cstr, strlen(cstr)) : stringview_null; }

stringview stringview_slice(stringview sv, int start, int end)
{
    if(start < 0) start += sv.size + 1;
    if(end < 0) end += sv.size + 1;

    if((start < 0) || (start > (int)sv.size) || (end < start) || (end < 0) || (end > (int)sv.size))
        return stringview_null;

    return stringview_newsize(sv.data, (size_t)(end - start));
}

bool _stringview_equal(stringview sv1, stringview sv2)
{
    if(sv1.size != sv2.size) return false;
    return !strncmp(sv1.data, sv2.data, sv1.size);
}

int _stringview_indexchar(stringview sv, char ch) { return _cstring_indexchar(sv.data, sv.size, ch); }
int _stringview_indexstringview(stringview sv, stringview strv) { return _cstring_index(sv.data, sv.size, strv.data, strv.size); }
int _stringview_indexcstring(stringview sv, const char* cstr) { return _cstring_index(sv.data, sv.size, cstr, strlen(cstr)); }
int _stringview_lastindexchar(stringview sv, char ch) { return _cstring_lastindexchar(sv.data, sv.size, ch); }
int _stringview_lastindexstringview(stringview sv, stringview strv) { return _cstring_lastindex(sv.data, sv.size, strv.data, strv.size); }
int _stringview_lastindexcstring(stringview sv, const char* cstr) { return _cstring_lastindex(sv.data, sv.size, cstr, strlen(cstr)); }
bool _stringview_startswithchar(stringview sv, char ch) { return _cstring_startswithchar(sv.data, sv.size, ch); }
bool _stringview_startswithcstring(stringview sv, const char* cstr) { return _cstring_startswith(sv.data, sv.size, cstr, strlen(cstr)); }
bool _stringview_startswithstringview(stringview sv, stringview strv) { return _cstring_startswith(sv.data, sv.size, strv.data, strv.size); }
bool _stringview_endswithchar(stringview sv, char ch) { return _cstring_endswithchar(sv.data, sv.size, ch); }
bool _stringview_endswithcstring(stringview sv, const char* cstr) { return _cstring_endswith(sv.data, sv.size, cstr, strlen(cstr)); }
bool _stringview_endswithstringview(stringview sv, stringview strv) { return _cstring_endswith(sv.data, sv.size, strv.data, strv.size); }
