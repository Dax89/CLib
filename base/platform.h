#pragma once

// https://stackoverflow.com/questions/142508/how-do-i-check-os-with-a-preprocessor-directive
// https://sourceforge.net/p/predef/wiki/OperatingSystems

#include <assert.h>

#if defined(__linux__)
    #define clib_platform_linux
    #define clib_platform_posix
#elif defined(__FreeBSD__)
    #define clib_platform_freebsd
    #define clib_platform_posix
#elif defined(__APPLE__) || defined(__MACH__)
    #define clib_platform_macosx
    #define clib_platform_posix
#elif defined(unix) || defined(__unix) || defined(__unix__)
    #define clib_platform_unix
    #define clib_platform_posix
#elif defined(_WIN32)
    #define clib_platform_windows
    #define clib_platform_windows32
#elif defined(_WIN64)
    #define clib_platform_windows
    #define clib_platform_windows64
#elif defined(__ANDROID__)
    #define clib_platform_android
    #define clib_platform_android_api __ANDROID_API__
    #define clib_platform_posix
#else
    static_assert(false, "Unsupported platform")
#endif

#if defined(__GNUC__) || defined(__GNUG__)
    #define clib_compiler_gnu
#elif defined(__clang__)
    #define clib_compiler_clang
#elif defined(__MINGW32__)
    #define clib_compiler_mingw
    #define clib_compiler_mingw32
#elif defined(__MINGW64__)
    #define clib_compiler_mingw
    #define clib_compiler_mingw64
#elif defined(_MSC_VER)
    #define clib_compiler_msvc
#else
    static_assert(false, "Unsupported compiler")
#endif
