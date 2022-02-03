#include "format.h"
#include "../containers/dict.h"
#include "../fs/path.h"
#include "stringview.h"
#include "string.h"
#include <stdarg.h>

#define format_basesize 1024

#define _format_integer(base) \
    static void _format_integer##base(string* str, va_list* args) { \
        string s = string_fromsigned(va_arg(*args, int), base); \
        string_append(str, s); \
        string_delete(&s); \
    }

static dict formats;
static bool initialized = false;

typedef struct format_node {
    format_cb cb;
    dict_nodeitem(const char*);
} format_node;

static void _format_integersigned(string* str, va_list* args) {
    string s = string_fromsigned(va_arg(*args, int), 10);
    string_append(str, s);
    string_delete(&s);
}

static void _format_integerunsigned(string* str, va_list* args) {
    string s = string_fromunsigned(va_arg(*args, unsigned int), 10);
    string_append(str, s);
    string_delete(&s);
}

_format_integer(2)
_format_integer(8)
_format_integer(16)

static void _format_char(string* str, va_list* args) { string_append(str, (char)va_arg(*args, int)); }
static void _format_cstring(string* str, va_list* args) { string_append(str, va_arg(*args, const char*)); }
static void _format_string(string* str, va_list* args) { string_append(str, *va_arg(*args, string*)); }
static void _format_stringview(string* str, va_list* args) { string_append(str, *va_arg(*args, stringview*)); }
static void _format_path(string* str, va_list* args) { string_append(str, path_stringview(*va_arg(*args, path*))); }

static void _format_initialize()
{
    if(initialized) return;
    initialized = true;

    formats = dict_new(format_node);

    // Basic Types
    dict_set(&formats, format_node, {.key = "c", .cb = _format_char});
    dict_set(&formats, format_node, {.key = "s", .cb = _format_cstring});
    dict_set(&formats, format_node, {.key = "b", .cb = _format_integer2});
    dict_set(&formats, format_node, {.key = "o", .cb = _format_integer8});
    dict_set(&formats, format_node, {.key = "x", .cb = _format_integer16});
    dict_set(&formats, format_node, {.key = "d", .cb = _format_integersigned});
    dict_set(&formats, format_node, {.key = "u", .cb = _format_integerunsigned});

    // CLib Types
    dict_set(&formats, format_node, {.key = "str", .cb = _format_string});
    dict_set(&formats, format_node, {.key = "strview", .cb = _format_stringview});
    dict_set(&formats, format_node, {.key = "path", .cb = _format_path});
}

void format_register(const char* name, format_cb cb)
{
    _format_initialize();
    dict_set(&formats, format_node, {.key = name, .cb = cb});
}

string format_full(const allocator* a, const char* fmt, ...)
{
    va_list args;
    string s;

    scope(va_start(args, fmt), va_end(args))
        s = _format_args(a, fmt, args);

    return s;
}

string _format_args(const allocator* a, const char* fmt, va_list args)
{
    _format_initialize();

    const char* p = fmt;
    string s = string_newfull(format_basesize, a), fmtname = string_new();

    defer(string_delete(&fmtname))
    {
        while(*p)
        {
            if(*p != '{' || (*p == '{' && *(p + 1) == '{'))
            {
                string_append(&s, *p++);
                continue;
            }

            const char* f = ++p, *end = f;

            while(*p)
            {
                end = ++p;

                if(*p == '}')
                {
                    p++;
                    break;
                }
            }

            string_copy(&fmtname, f, end - f);
            const dict_node* fn = dict_get(formats, format_node, string_ptr(fmtname));

            if(!fn)
            {
                string_delete(&s);
                break;
            }
            else
                dict_entry(format_node, fn)->cb(&s, (va_list*)&args);
        }
    }

    return s;
}
