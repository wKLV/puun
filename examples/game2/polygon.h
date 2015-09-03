#ifndef puun_polygonh
#define puun_polygonh
#include <puun/types.h>
#include "vector.h"

#if 0 //TODO Perhaps we want this sometime?
#if puun_2D
typedef v2 Vertex;
#else
typedef v3 Vertex;
#endif
#endif
typedef v2 Vertex;

typedef union {
  Vertex vertices [2];
  struct {
    Vertex beggining;
    Vertex end;
  };
} Edge;

typedef union {
  Vertex vertices [3];
  struct {
    Vertex v1;
    Vertex v2;
    Vertex v3;
  };
} Triangle;

typedef ARRAY_DEFINE(Vertex) Polygon;


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

PolygonList create_polygonList(u8 program, Polygon* polygons);

void polygonList_update_pos(PolygonList* sl, Data d, Data d2);

void render_polygonList(PolygonList p, Data* unis, s32 unis_length);

typedef struct {
    DEF_ARR(triangles, Triangle)

    u8 program;
    u32 pos_buffer;
    u32 vertexCount;
} TriangleList;

TriangleList triange_list_from_polygon(u8 program, Polygon* p, s32 polygon_count, Data d, Data d2); 

TriangleList create_triangleList(u8 program, Triangle* triangles);

void triangleList_update_pos(TriangleList sl, Data d);

void render_triangleList(TriangleList p, Data* unis, s32 unis_length);
#endif

