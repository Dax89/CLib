#include "print.h"
#include "../string/format.h"
#include "../string/string.h"
#include <stdio.h>
#include <stdarg.h>

static void _print(const char* restrict fmt, bool newline, va_list args)
{
    string s = _format_args(default_allocator, fmt, args);

    if(!string_isnull(s))
    {
        fputs(string_ptr(s), stdout);
        if(newline) fputs("\n", stdout);
    }
    else fputs("Invalid string format\n", stdout);
    string_delete(&s);
}

void print(const char* fmt, ...)
{
    va_list args;
    scope(va_start(args, fmt), va_end(args))
        _print(fmt, false, args);
}

void println(const char* fmt, ...)
{
    va_list args;
    scope(va_start(args, fmt), va_end(args))
        _print(fmt, true, args);
}

string read()
{
    int ch = 0;
    string s = string_newsize(string_defaultcapacity);

    while((ch = fgetc(stdin)) != EOF)
    {
        if(ch == '\n') break;
        string_append(&s, ch);
    }

    return s;
}
