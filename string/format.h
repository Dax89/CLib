#pragma once

#include "../base/allocator.h"
#include <stdarg.h>

struct string;

#define format(fmt, ...) format_full(default_allocator, fmt, __VA_ARGS__)

typedef void (*format_cb)(struct string* s, va_list args);

void format_register(const char* name, format_cb cb);
struct string format_full(const allocator* a, const char* restrict fmt, ...);

struct string _format_args(const allocator* a, const char* restrict fmt, va_list args);
