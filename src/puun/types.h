#ifndef puun_typesh
#define puun_typesh

#include <stdint.h>
#include <stdbool.h>

typedef void* Data;
typedef char* String;
typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef int8_t s8;
typedef int16_t s16;
typedef int32_t s32;
typedef int64_t s64;

typedef int8_t b8;
typedef int16_t b16;
typedef int32_t b32;
typedef int64_t b64;

typedef float f32;
typedef double f64;


// ARRAY HELPER

#define ARRAY(x) struct ARRAY_ ## x
#define ARRAY_DEFINE(x) struct ARRAY_ ## x {x* value; s32 num;}

#define PARAM_ARR(name, x) x* name, s32 name ## _length
#define DEF_ARR(name, x) x* name; s32 name ## _length;

typedef ARRAY_DEFINE(Data) DataArray;

// gx types
typedef struct {
    f32 x;
    f32 y;
    f32 w;
    f32 h;
} BBox;
#endif
