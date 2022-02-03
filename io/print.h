#pragma once

#include "../base/allocator.h"
#include "../string/string.h"

void print(const char* restrict fmt, ...);
void println(const char* restrict fmt, ...);
string readfull(const allocator* a);

static inline string read() { return readfull(default_allocator); }
