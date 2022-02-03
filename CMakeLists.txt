cmake_minimum_required(VERSION 3.5)

project(CLib)

option(CLIB_ENABLE_ASAN "Enable Address Sanitizer" OFF)

SET(PRIVATE_HEADERS base/private/string.h)
SET(PRIVATE_SOURCES base/private/string.c)

SET(HEADERS base/platform.h base/common.h base/base.h base/hash.h base/allocator.h
            string/stringview.h string/string.h string/format.h
            containers/vector.h containers/list.h containers/dict.h
            io/print.h io/file.h
            fs/path.h fs/filesystem.h
            clib.h)

SET(SOURCES base/base.c base/hash.c base/allocator.c
            string/stringview.c string/string.c string/format.c
            containers/vector.c containers/list.c containers/dict.c
            io/print.c io/file.c
            fs/path.c fs/filesystem.c
            clib.c)

add_library( ${PROJECT_NAME} STATIC ${PRIVATE_HEADERS} ${PRIVATE_SOURCES} ${HEADERS} ${SOURCES})
target_compile_features(${PROJECT_NAME} PUBLIC c_std_11)

if(MSVC)
    target_compile_options(${PROJECT_NAME} PUBLIC /W3 /WX)

    if(CLIB_ENABLE_ASAN)
        target_compile_options( ${PROJECT_NAME} PUBLIC /fsanitize=address)
    endif()
else()
    target_compile_options(${PROJECT_NAME} PUBLIC -Wall -Wextra -Wpedantic -Werror)

    if(CLIB_ENABLE_ASAN)
        target_compile_options( ${PROJECT_NAME} PUBLIC -fsanitize=address -fno-optimize-sibling-calls -fsanitize-address-use-after-scope -fno-omit-frame-pointer -g -O0)
    endif()
endif()

