#ifndef puun_typesh
#define puun_typesh

#include <stdint.h>
#include <stdbool.h>

typedef void* Data;
typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef int8_t s8;
typedef int16_t s16;
typedef int32_t s32;
typedef int64_t s64;

typedef int32_t bool32;

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
    u32 x;
    u32 y;
    u32 w;
    u32 h;
} BBox;
#endif
