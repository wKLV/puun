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

typedef uint8_t b8;
typedef uint16_t b16;
typedef uint32_t b32;
typedef uint64_t b64;

typedef float f32;
typedef double f64;

// STRUCT INITIALIZER
//#ifdef MSVC
#if _MSC_VER
#define ZERO_STRUCT {0}
#else
#define ZERO_STRUCT {}
#endif

// MATH STUFF
#define TAU 6.28318530717958647692

#ifndef _MSC_VER
#define min(x,y) (((x)<(y))?(x):(y))
#define max(x,y) (((x)>(y))?(x):(y))
#endif

#define clamp(x, min, max) (((x)>(max))?(max):(((x)<(min))?(min):(x)))


// ARRAY HELPER

#define ARRAY(x) struct ARRAY_ ## x
#define ARRAY_DEFINE(x) struct ARRAY_ ## x {x* value; s32 num;}

#define PARAM_ARR(name, x) x* name, s32 name ## _length
#define DEF_ARR(name, x) x* name; s32 name ## _length;

#define ARRCOUNT(x) (sizeof((x))/sizeof((x)[0]))

typedef ARRAY_DEFINE(Data) DataArray;

//COMMON DATA STRUCTURES
#define DOUBLE_LINKED_LIST(type)\
struct type##_Linked_List{\
    struct type##_Linked_List* next;\
    struct type##_Linked_List* previous;\
    type value;\
}

// gx types
typedef struct {
    f32 x;
    f32 y;
    f32 w;
    f32 h;
} BBox;
#endif
