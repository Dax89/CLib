#include "string.h"
#include <string.h>

int _cstring_indexchar(const char* s1, size_t sz, char ch)
{
    for(int i = 0; i < (int)sz; i++)
    {
        if(s1[i] == ch)
            return i;
    }

    return -1;
}

int _cstring_index(const char* s1, size_t sz1, const char* s2, size_t sz2)
{
    if(sz1 < sz2) return -1;

    for(size_t i = 0; i < sz1; i++)
    {
        bool found = true;

        for(size_t j = 0; j < sz2; j++)
        {
            if(s1[i + j] == s2[j]) continue;

            found = false;
            break;
        }

        if(found) return i;
    }

    return -1;
}

int _cstring_lastindexchar(const char* s1, size_t sz, char ch)
{
    for(int i = (int)sz; i-- > 0; )
    {
        if(s1[i] == ch)
            return i;
    }

    return -1;
}

int _cstring_lastindex(const char* s1, size_t sz1, const char* s2, size_t sz2)
{
    if(sz1 < sz2) return -1;

    for(int i = sz1; i-- > 0; )
    {
        bool found = true;

        for(size_t j = 0; j < sz2; j++)
        {
            if(s1[i + j] == s2[j]) continue;

            found = false;
            break;
        }

        if(found) return i;
    }

    return -1;
}

bool _cstring_startswithchar(const char* s1, size_t sz, char ch) { return sz && (s1[0] == ch); }
bool _cstring_startswith(const char* s1, size_t sz1, const char* s2, size_t sz2) { return (sz1 > sz2) && !strncmp(s1, s2, sz2); }
bool _cstring_endswithchar(const char* s1, size_t sz, char ch) { return sz && (s1[sz - 1] == ch); }
bool _cstring_endswith(const char* s1, size_t sz1, const char* s2, size_t sz2) { return (sz1 > sz2) && !strcmp(s1 + sz1 - sz2, s2); }

bool _cstring_splitnext(const char** s1, size_t* sz1, const char* s2, size_t sz2, char sep)
{
    const char *endp = s2 + sz2, *p = *s1 ? (*s1 + *sz1 + 1) : s2;
    if(p >= endp) return false;

    const char* startp = p;
    while((p < endp) && (*p != sep)) p++;

    *s1 = startp;
    *sz1 = (p - startp);
    return true;
}
