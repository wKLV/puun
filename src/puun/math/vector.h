#include <puun/math_types.h>
#include "math.h"
v2 normalized_v2(v2 v) {
     v2 result = ZERO_STRUCT;
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
