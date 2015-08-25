#ifndef puun_polygonh
#define puun_polygonh
#include "../puun/types.h"
#include "vector.h"

typedef ARRAY_DEFINE(v2) Polygon;

b32 intersect_polygon(Polygon a, Polygon b);

typedef struct {
   // Square* squares;
   // s32 squares_length;
    DEF_ARR(polygons, Polygon)

    u8 program;
    u32 pos_buffer;
    u32 els_buffer;
    u32 vertexCount;
} PolygonList;

PolygonList create_polygonList(u8 program, Polygon* squares);

void polygonList_update_pos(PolygonList* sl, Data d, Data d2);

void render_polygonList(PolygonList p, Data* unis, s32 unis_length);

#endif

