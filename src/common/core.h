#pragma once

#include <stdint.h>

#ifdef _MSC_VER
    #define ASSERT(X, ...) if(!(X)) {printf(__VA_ARGS__), __debugbreak();}
#else
    #define ASSERT(X, ...) if(!(X)){printf(__VA_ARGS__), *(int*)0 = 1234;}
#endif

typedef uint8_t uint8;
typedef uint32_t uint32;
typedef uint16_t uint16;
typedef uint64_t uint64;

typedef int8_t int8;
typedef int16_t int16;
typedef int32_t int32;
typedef int64_t int64;