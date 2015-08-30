#ifndef puun_vectorh
#define puun_vectorh
typedef union V2 {
    struct {
        float x, y;
    };
    struct {
        float u, v;
    };
    float els[2];
} v2;

v2 new_v2(float x, float y) {
    v2 result = {};
    result.x = x;
    result.y = y;
    return result;
}

typedef union V3 {
    struct {
        float x, y, z;
    };
    struct {
        float r, g, b;
    };
    float els[3];
} v3;

v3 new_v3(float x, float y, float z) {
    v3 result = {};
    result.x = x;
    result.y = y;
    result.z = z;
    return result;
}

typedef union V4 {
    struct {
        float x, y, z, w;
    };
    struct {
        float r, g, b, a;
    };
    float els[4];
} v4;
v4 new_v4(float x, float y, float z, float w) {
    v4 result = {};
    result.x = x;
    result.y = y;
    result.z = z;
    result.w = w;
    return result;
}
#define puun_create_vector(n) \
v##n add_v##n(v##n a, v##n b);\
v##n sub_v##n(v##n a, v##n b);\
v##n minus_v##n(v##n v);\
v##n scalar_v##n(v##n v, float s);\
float dot_v##n(v##n a, v##n b);\
float lengthSq_v##n(v##n v);\
v##n normalized_v##n(v##n v);\
\
v##n add_v##n(v##n a, v##n b) {\
    v##n result = {};\
    int i;\
    for(i=0; i<n;++i)\
        result.els[i] = a.els[i] +b.els[i];\
    return result;\
}\
\
v##n minus_v##n(v##n v) {\
     v##n result = {};\
    int i;\
    for(i=0; i<n;++i)\
        result.els[i] = - v.els[i];\
     return result;\
}\
\
v##n sub_v##n(v##n a, v##n b) {\
    v##n result = {};\
    b = minus_v##n(b);\
    result = add_v##n(a, b);\
    return result;\
}\
\
v##n scalar_v##n(v##n v, float s) {\
     v##n result = v;\
    int i;\
    for(i=0; i<n;++i)\
        result.els[i] *= s;\
     return result;\
}\
float dot_v##n(v##n a, v##n b) {\
    float result = 0;\
    int i;\
    for(i=0; i<n;++i)\
        result += a.els[i]*b.els[i];\
    return result;\
}\
\
float lengthSq_v##n(v##n v) {\
    float result = 0;\
    result = dot_v##n(v, v);\
    return result;\
}\
b32 isValid_v##n(v##n v) {\
    b32 result = true;\
    int i;\
    for(i=0; i<n; ++i) {\
        result &= v.els[i] == v.els[i];\
    }\
    return result;\
}

puun_create_vector(2);
puun_create_vector(3);
puun_create_vector(4);

#include "math.h"
#if 1
v2 normalized_v2(v2 v) {
     v2 result = {};
     float length = lengthSq_v2(v);
     length = sqrtf(length);
     assert(length);
     float invLength;
     invLength = 1.f/length;
     result = scalar_v2(v, invLength);
     assert(isValid_v2(result));
     return result;
}

float angle_between_ABC(v2 a, v2 b, v2 c) {
    float result;
    v2 ab = sub_v2(b, a);
    v2 cb = sub_v2(b, c);

    float dot = dot_v2(ab, cb);
    float cross = ab.x *cb.y - ab.y * cb.x;

    result = atan2f(cross, dot);
    result = -result;
    result += TAU;
    result = fmod(result, TAU);
    return result; 
}

//TODO Optimize this or something D:
float angle_between_v2(v2 a, v2 b) {
    float result = 0;
    v2 normalizedA = normalized_v2(a);
    v2 normalizedB = normalized_v2(b);
    result = dot_v2(normalizedA, normalizedB);
    assert(result==result);
    result = clamp(result, -1.f, 1.f);
    result = acosf(result);
    assert(result==result);

    v2 subs = sub_v2(a, b);
    result = atan2f(subs.y, subs.x);
   /* while(aV > TAU) aV-= TAU;
    while(aV < 0) aV+= TAU;
    float bV = atan2f(b.y, b.x);
    while(bV > TAU) bV-= TAU;
    while(bV < 0) bV+= TAU;
    result = bV - aV;
    */
    while(result > TAU) result-= TAU;
    while(result < 0) result+= TAU;

    return result;
}
#endif

#endif
