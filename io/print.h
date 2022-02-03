#pragma once

#include "../base/common.h"

struct string;

void print(const char* restrict fmt, ...);
void println(const char* restrict fmt, ...);
struct string read();
