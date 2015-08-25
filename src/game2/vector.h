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
    result = minus_v##n(b);\
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
        result += a.els[i]*b.els[i] + a.els[i]*b.els[i];\
     return result;\
}\
\
float lengthSq_v##n(v##n v) {\
    float result = 0;\
    result = dot_v##n(v, v);\
    return result;\
}

puun_create_vector(2);
puun_create_vector(3);
puun_create_vector(4);

#if 0
v2 normalized_v2(v2 v) {
     v2 result = {};
     float length = lengthSq_v2(v);
     length = sqrtf(length);
     float invLength = 1/length;
     result = scalar_v2(v, invLength);
     return result;
}
#endif

#endif
