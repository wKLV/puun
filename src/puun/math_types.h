#ifndef puun_mathh
#define puun_mathh
#include "types.h"

#include <math.h>

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

typedef union M2 {
    v2 els[2];
    f32 f_els[4];
    struct {
        f32 a0; f32 a1;
        f32 b0; f32 b1;
    };
    struct {
        v2 a; v2 b;
    };
} m2;

typedef union M3 {
    v3 els[3];
    f32 f_els[9];
    struct {
        f32 a0; f32 a1; f32 a2;
        f32 b0; f32 b1; f32 b2;
        f32 c0; f32 c1; f32 c2;
    };
    struct {
        v3 a; v3 b; v3 c;
    };
} m3;

typedef union M4 {
    v4 els[4];
    f32 f_els[16];
    struct {
        f32 a0; f32 a1; f32 a2; f32 a3;
        f32 b0; f32 b1; f32 b2; f32 b3;
        f32 c0; f32 c1; f32 c2; f32 c3;
        f32 d0; f32 d1; f32 d2; f32 d3;
    };
    struct {
        v4 a; v4 b; v4 c; v4 d;
    };
} m4;

m2 identity_m2();
m3 identity_m3();
m4 identity_m4();

v2 row_m2(m2 m, i32 row);
v3 row_m3(m3 m, i32 row);
v4 row_m4(m4 m, i32 row);

v2 col_m2(m2 m, i32 row);
v3 col_m3(m3 m, i32 row);
v4 col_m4(m4 m, i32 row);

m3 m3_from_m2(m2 m);
m2 m2_from_m3(m3 m);

m3 m3_from_m4(m4 m);
m4 m4_from_m3(m3 m);

m2 rot_angle(f32 angle);
m3 rot_angle_m3(f32 angle);
m3 rot_axis_angle(v3 axis, f32 angle);
m4 rot_axis_angle_m4(v3 axis, f32 angle);

m3 traslate_m3(v2 d);
m4 traslate_m4(v3 d);

m3 scale_m3(f32 scale);
m4 scale_m4(f32 scale);

#define puun_define_matrix(n) \
m##n add_m##n(m##n a, m##n b);\
m##n sub_m##n(m##n a, m##n b);\
m##n minus_m##n(m##n m);\
m##n scalar_m##n(m##n m, float s);\
m##n append_m##n(m##n a, m##n b);\
m##n normalized_m##n(m##n m);\
b32 isValid_m##n(m##n v);

puun_define_matrix(2);
puun_define_matrix(3);
puun_define_matrix(4);

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
v##n normalized_v##n(v##n v) {\
    v##n result = ZERO_STRUCT;\
    f32 length = sqrtf(lengthSq_v##n(v));\
    result = scalar_v##n(v, 1/length);\
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

#define puun_create_matrix(n) \
m##n add_m##n(m##n a, m##n b) {\
    m##n result = ZERO_STRUCT;\
    i32 i; i32 j;\
    for(i=0; i<n; ++i) for(j=0; j<n; ++j) {\
        result.els[i].els[j] = a.els[i].els[j] + b.els[i].els[j];\
    }\
    return result;\
}\
m##n minus_m##n(m##n m) {\
    m##n result = ZERO_STRUCT;\
    i32 i; i32 j;\
    for(i=0; i<n; ++i) for(j=0; j<n; ++j) {\
        result.els[i].els[j] = - m.els[i].els[j];\
    }\
    return result;\
}\
m##n sub_m##n(m##n a, m##n b) {\
    m##n result = ZERO_STRUCT;\
    b = minus_m##n(b);\
    result = add_m##n(a, b);\
    return result;\
}\
m##n scalar_m##n(m##n m, float s) {\
    m##n result = ZERO_STRUCT;\
    i32 i; i32 j;\
    for(i=0; i<n; ++i) for(j=0; j<n; ++j) {\
        result.els[i].els[j] = s * m.els[i].els[j];\
    }\
    return result;\
}\
m##n append_m##n(m##n a, m##n b) {\
    m##n result = ZERO_STRUCT;\
    i32 i; i32 j;\
    for(i=0; i<n; ++i) for(j=0; j<n; ++j) {\
        result.els[i].els[j] = dot_v##n(row_m##n(a, i), col_m##n(b, j));\
    }\
    return result;\
}\
b32 isValid_m##n(m##n v) {\
   b32 result = true;\
   i32 i; i32 j;\
   for(i=0; i<n; ++i) for(j=0; j<n; ++j) {\
        result &= v.els[i].els[j] == v.els[i].els[j];\
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

m2 identity_m2() {
    m2 result = ZERO_STRUCT;
    result.a0 =1; result.b1 = 1;
    return result;
}

m3 identity_m3() {
    m3 result = ZERO_STRUCT;
    result.a0 = 1; result.b1=1; result.c2 =1;
    return result;
}

m4 identity_m4() {
    m4 result = ZERO_STRUCT;
    result.a0 = 1; result.b1=1; result.c2 =1; result.d3=1;
    return result;
}

v2 row_m2(m2 m, i32 row) {
    v2 result = ZERO_STRUCT;
    i32 i;
    for(i=0; i<2; ++i) {
        result.els[i] = m.els[row].els[i];
    }
    return result;

}

v3 row_m3(m3 m, i32 row) {
    v3 result = ZERO_STRUCT;
    i32 i;
    for(i=0; i<3; ++i) {
        result.els[i] = m.els[row].els[i];
    }
    return result;

}

v4 row_m4(m4 m, i32 row) {
    v4 result = ZERO_STRUCT;
    i32 i;
    for(i=0; i<4; ++i) {
        result.els[i] = m.els[row].els[i];
    }
    return result;
}

v2 col_m2(m2 m, i32 col) {
    v2 result = ZERO_STRUCT;
    i32 i;
    for(i=0; i<2; ++i) {
        result.els[i] = m.els[i].els[col];
    }
    return result;

}

v3 col_m3(m3 m, i32 col) {
    v3 result = ZERO_STRUCT;
    i32 i;
    for(i=0; i<3; ++i) {
        result.els[i] = m.els[i].els[col];
    }
    return result;

}

v4 col_m4(m4 m, i32 col) {
    v4 result = ZERO_STRUCT;
    i32 i;
    for(i=0; i<4; ++i) {
        result.els[i] = m.els[i].els[col];
    }
    return result;
}

m3 m3_from_m2(m2 m) {
    m3 result = identity_m3();
    i32 i; i32 j;
    for(i=0; i<2; ++i) for(j=0; j<2; ++j) {
        result.els[i].els[j] = m.els[i].els[j];
    }
    return result;
}

m2 m2_from_m3(m3 m) {
    m2 result = ZERO_STRUCT; 
    i32 i; i32 j;
    for(i=0; i<2; ++i) for(j=0; j<2; ++j) {
        result.els[i].els[j] = m.els[i].els[j];
    }
    return result;
}

m3 m3_from_m4(m4 m) {
    m3 result = identity_m3();
    i32 i; i32 j;
    for(i=0; i<3; ++i) for(j=0; j<3; ++j) {
        result.els[i].els[j] = m.els[i].els[j];
    }
    return result;
}
m4 m4_from_m3(m3 m) {
    m4 result = identity_m4();
    i32 i; i32 j;
    for(i=0; i<3; ++i) for(j=0; j<3; ++j) {
        result.els[i].els[j] = m.els[i].els[j];
    }
    return result;
}


puun_create_vector(2);
puun_create_vector(3);
puun_create_vector(4);

puun_create_matrix(2);
puun_create_matrix(3);
puun_create_matrix(4);


m2 rot_angle(f32 angle) {
    m2 result = ZERO_STRUCT;
    f32 co = cos(angle); f32 si = sin(angle);
    result.a0 = co;
    result.a1 = si;
    result.b0 = -si;
    result.b1 = co;
    return result;
}


m3 rot_angle_m3(f32 angle) {
    m3 result = ZERO_STRUCT;
    result = m3_from_m2(rot_angle(angle));
    return result;
}

m3 rot_axis_angle(v3 axis, f32 angle) {
    m3 result = ZERO_STRUCT;
    axis = normalized_v3(axis);
    f32 co = cos(angle), si = sin(angle), mco = 1 -co;
    f32 x = axis.x, y = axis.y, z = axis.z;
    
    result.a0 = co + x*x*mco;
    result.a1 = y*x*mco + z*si;
    result.a2 = z*x*mco - y*si;

    result.b0 = x*y*mco - z*si;
    result.b1 = co + y*y*mco;
    result.b2 = z*y*mco + x*si;

    result.c0 = x*y*mco + y*si;
    result.c1 = y*z*mco - x*si;
    result.c2 = co + z*z*mco;
    return result;
}

m4 rot_axis_angle_m4(v3 axis, f32 angle) {
    m4 result = ZERO_STRUCT;
    result = m4_from_m3(rot_axis_angle(axis, angle));
    return result;
}

m3 traslate_m3(v2 d) {
    m3 result = identity_m3();
    i32 i;
    for(i=0; i<2; ++i) {
        result.c.els[i] = d.els[i];
    }
    return result;
}
m4 traslate_m4(v3 d) {
    m4 result = identity_m4();
    i32 i;
    for(i=0; i<3; ++i) {
        result.d.els[i] = d.els[i];
    }
    return result; 
}

m3 scale_m3(f32 scale) {
    m3 result = ZERO_STRUCT;
    m2 sub = identity_m2();
    sub = scalar_m2(sub, scale);
    result = m3_from_m2(sub);
    return result; 
}

m4 scale_m4(f32 scale) {
    m4 result = ZERO_STRUCT;
    m3 sub = identity_m3();
    sub = scalar_m3(sub, scale);
    result = m4_from_m3(sub);
    return result; 
}

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
