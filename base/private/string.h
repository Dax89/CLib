#pragma once

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

int _cstring_indexchar(const char* s1, size_t sz, char ch);
int _cstring_index(const char* s1, size_t sz1, const char* s2, size_t sz2);
int _cstring_lastindexchar(const char* s1, size_t sz, char ch);
int _cstring_lastindex(const char* s1, size_t sz1, const char* s2, size_t sz2);

bool _cstring_startswithchar(const char* s1, size_t sz, char ch);
bool _cstring_startswith(const char* s1, size_t sz1, const char* s2, size_t sz2);
bool _cstring_endswithchar(const char* s1, size_t sz, char ch);
bool _cstring_endswith(const char* s1, size_t sz1, const char* s2, size_t sz2);

bool _cstring_splitnext(const char** s1, size_t* sz1, const char* s2, size_t sz2, char sep);
