#ifndef puun_typesh
#define puun_typesh

#include <stdint.h>
#include <stdbool.h>

typedef void* Data;
typedef char* String;
#define STRING(name, n) char name[n];
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

#define KIBIBYTES(x) ((1 << 10)*(x))
#define MEBIBYTES(x) ((1<<10) * KIBIBYTES(x))
#define GIBIBYTES(x) ((1<<10) * MEBIBYTES(x))

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
#define clamp01(x) clamp((x), 0, 1)

#define lerp(x, y, t) ((x)*(t) + (1-(t))*(y))
#define quadlerp(x, y, t) lerp((x), (y), (t)*(t)) 

// ARRAY HELPER

#define ARRAY(x) struct ARRAY_ ## x
#define ARRAY_DEFINE(x) struct ARRAY_ ## x {x* value; s32 num;}

#define PARAM_ARR(name, x) x* name, s32 name ## _length
#define DEF_ARR(name, x) x* name; s32 name ## _length;

#define ARRCOUNT(x) (sizeof((x))/sizeof((x)[0]))

typedef ARRAY_DEFINE(Data) DataArray;

//COMMON DATA STRUCTURES
#define LINKED_LIST(type)\
struct type##_Linked_List{\
    type value;\
    struct type##_Linked_List* next;\
}

#define DOUBLE_LINKED_LIST(type)\
struct type##_DoubleLinked_List{\
    type value;\
    struct type##_DoubleLinked_List* next;\
    struct type##_DoubleLinked_List* previous;\
}
#endif
