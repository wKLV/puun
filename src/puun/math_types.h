#ifndef puun_mathh
#define puun_mathh
#include "types.h"
typedef union V2 {
    struct {
        float x, y;
    };
    struct {
        float u, v;
    };
    float els[2];
} v2;

typedef union V3 {
    struct {
        float x, y, z;
    };
    struct {
        float r, g, b;
    };
    float els[3];
} v3;

typedef union V4 {
    struct {
        float x, y, z, w;
    };
    struct {
        float r, g, b, a;
    };
    float els[4];
} v4;

#define puun_define_vector(n) \
v##n add_v##n(v##n a, v##n b);\
v##n sub_v##n(v##n a, v##n b);\
v##n minus_v##n(v##n v);\
v##n scalar_v##n(v##n v, float s);\
float dot_v##n(v##n a, v##n b);\
float lengthSq_v##n(v##n v);\
v##n normalized_v##n(v##n v);\
b32 isValid_v##n(v##n v);

puun_define_vector(2);
puun_define_vector(3);
puun_define_vector(4);

v2 new_v2(float x, float y);
v3 new_v3(float x, float y, float z);
v4 new_v4(float x, float y, float z, float w);

typedef union {
    struct{
        f32 x1;
        f32 y1;
        f32 x2;
        f32 y2;
    };
    struct {
        v2 bottomLeft;
        v2 topRight; 
    };
} Rectangle;

Rectangle rectangle_topleft_to_bottomright(f32 x1, f32 y1, f32 x2, f32 y2);
Rectangle rectangle_topleft_by_widtheight(f32 x1, f32 y1, f32 w, f32 h);
Rectangle rectangle_center_by_halfwidtheight(f32 x0, f32 y0, f32 w2, f32 h2);
Rectangle rectangle_center_by_widtheight(f32 x0, f32 y0, f32 w, f32 h);
v2 widthheight_rectangle(Rectangle r);
f32 width_rectangle(Rectangle r);
f32 height_rectangle(Rectangle r);

#define puun_create_vector(n) \
v##n add_v##n(v##n a, v##n b) {\
    v##n result = ZERO_STRUCT;\
    int i;\
    for(i=0; i<n;++i)\
        result.els[i] = a.els[i] +b.els[i];\
    return result;\
}\
\
v##n minus_v##n(v##n v) {\
     v##n result = ZERO_STRUCT;\
    int i;\
    for(i=0; i<n;++i)\
        result.els[i] = - v.els[i];\
     return result;\
}\
\
v##n sub_v##n(v##n a, v##n b) {\
    v##n result = ZERO_STRUCT;\
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


#ifdef puun_implement_math
#if puun_implement_math

v2 new_v2(float x, float y) {
    v2 result = ZERO_STRUCT;
    result.x = x;
    result.y = y;
    return result;
}

v3 new_v3(float x, float y, float z) {
    v3 result = ZERO_STRUCT;
    result.x = x;
    result.y = y;
    result.z = z;
    return result;
}

v4 new_v4(float x, float y, float z, float w) {
    v4 result = ZERO_STRUCT;
    result.x = x;
    result.y = y;
    result.z = z;
    result.w = w;
    return result;
}

puun_create_vector(2);
puun_create_vector(3);
puun_create_vector(4);

Rectangle rectangle_topleft_to_bottomright(f32 x1, f32 y1, f32 x2, f32 y2) {
    Rectangle result = ZERO_STRUCT;
    result.x1 = x1;
    result.y1 = y1;
    result.x2 = x2;
    result.y2 = y2;
    return result;
}
Rectangle rectangle_topleft_by_widtheight(f32 x1, f32 y1, f32 w, f32 h) {
    Rectangle result = ZERO_STRUCT;
    result.x1 = x1;
    result.y1 = y1;
    result.x2 = x1+w;
    result.y2 = y1+h;
    return result;
}

Rectangle rectangle_center_by_halfwidtheight(f32 x0, f32 y0, f32 w2, f32 h2) {
    Rectangle result = ZERO_STRUCT;
    result.x1 = x0-w2;
    result.y1 = y0-h2;
    result.x2 = x0+w2;
    result.y2 = y0+h2;
    return result;
}

Rectangle rectangle_center_by_widtheight(f32 x0, f32 y0, f32 w, f32 h) {
    f32 w2 = w/2.f;
    f32 h2 = h/2.f;
    return rectangle_center_by_halfwidtheight(x0, y0, w2, h2);
}
v2 widthheight_rectangle(Rectangle r) {
    v2 result = sub_v2(r.topRight, r.bottomLeft);
    return result;
}

f32 width_rectangle(Rectangle r) {
   f32 result = widthheight_rectangle(r).x;
   return result;
}
f32 height_rectangle(Rectangle r)  {
   f32 result = widthheight_rectangle(r).y;
   return result;
}
    
#endif
#endif
#endif
