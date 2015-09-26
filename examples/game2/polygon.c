#include "GL/glew.h"
#include <assert.h>
#include <string.h>

#include <puun/gf/gl_help.h>

#include "polygon.h"
#include "vector.h"

b32 intersect_polygon(Polygon a, Polygon b) {
//TODO Implement
    return false;
}


PolygonList create_polygonList(u8 program, Polygon* polygons) {
    PolygonList pl = ZERO_STRUCT;
    pl.polygons = polygons;

    pl.polygons_length = 0;
    pl.program = program;
    glGenBuffers(1, &pl.pos_buffer);
    glGenBuffers(1, &pl.els_buffer);

    return pl;
}

// TODO: Do i want this or should I always triangulate?
void polygonList_update_pos(PolygonList* pl, Data d, Data d2) {
    float* data = (float*)d;
    int* els = (int*)d2;

    int i, dataI, elsI, verI;
    for(i=0, dataI=0, elsI=0, verI=0; i<pl->polygons_length; i++) {
        Polygon p = pl->polygons[i];
        int j;
        for(j=0; j<p.num; j++) {
            data[dataI++] = p.value[j].x;
            data[dataI++] = p.value[j].y;
        }
        //NOTE: triangulate naively polygon
        // TODO: Should I use the same code I use in triangulate for this?
        int polygonFirst = verI;
        int polygonGreat = polygonFirst;
        while(polygonGreat<polygonFirst+p.num-2) {
            els[elsI++] = polygonFirst;
            els[elsI++] = ++polygonGreat;
            els[elsI++] = polygonGreat + 1;
        }
        verI += p.num;
    }
    GLint pos = glGetAttribLocation(pl->program, "position");
    u8 stride = (2)*sizeof(float);

    glBindBuffer(GL_ARRAY_BUFFER, pl->pos_buffer);
    glBufferData(GL_ARRAY_BUFFER, dataI*sizeof(float), data, GL_STATIC_DRAW);
    glVertexAttribPointer(pos, 2, GL_FLOAT, GL_FALSE, stride, 0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, pl->els_buffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, elsI*sizeof(int), els, GL_STATIC_DRAW);
    pl->vertexCount = elsI;
}

void render_polygonList(PolygonList pl, Data* unis_untyped, s32 unis_length) {
    UniformData* unis = (UniformData*) unis_untyped;

    glUseProgram(pl.program);
    int i;
    for(i=0; i<unis_length; i++){
        setParam(pl.program, unis[i]);
    }

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, pl.els_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, pl.pos_buffer);
    GLint pos = glGetAttribLocation(pl.program, "position");
    glVertexAttribPointer(pos, 2, GL_FLOAT, GL_FALSE, 2*sizeof(float), 0);
    glEnableVertexAttribArray(pos);
    glDrawElements(GL_TRIANGLES, pl.vertexCount, GL_UNSIGNED_INT, (void*)0);
}

TriangleList create_triangleList(u8 program, Triangle* triangles) {
    TriangleList tl = ZERO_STRUCT;

    tl.triangles_length = 0;
    tl.triangles = triangles;
    tl.program = program;
    glGenBuffers(1, &tl.pos_buffer);

    return tl;
}

typedef DOUBLE_LINKED_LIST(Vertex) Vertex_Doubled_Link;
#if _MSC_VER
//NOTE: 100 vertices ought to be sufficient for anyone
//NOTE: this is because msc can't make an array of length varying on instance
#define SUFFICIENTLYLARGENUMBER 100
#else
#define SUFFICIENTLYLARGENUMBER p.num
#endif

TriangleList triange_list_from_polygon(u8 program, Polygon* ps, s32 polygon_count, Data d, Data d2) {
    Triangle* outData = (Triangle*)d;

    int i, dataI;
    for(i=0, dataI=0; i<polygon_count; ++i)
    {
        // TODO: Should I pull this to triangule_polygon?
        Polygon p = ps[i];
        Vertex_Doubled_Link verts[SUFFICIENTLYLARGENUMBER];
        int j;
        for(j=0; j<p.num; ++j) {
            Vertex_Doubled_Link* v = &verts[j];
            v->value = p.value[j];
            v->next = verts + ((j + 1) % p.num);
            //TODO I'm not using previous. Should delete and make a note?
            v->previous = verts + ((j +p.num - 1) % p.num);
        }
        Vertex_Doubled_Link* currentVertex = &verts[0];
        b32 isError = false;
        Vertex_Doubled_Link* first[SUFFICIENTLYLARGENUMBER];
        first[0] = currentVertex;
        s32 manyFirst = 1;
        b32 isFirstRound[SUFFICIENTLYLARGENUMBER];
        for(j=0; j<p.num; ++j) {
            isFirstRound[j] = true;
        }
        while(currentVertex->next != currentVertex
            && currentVertex->next->next != currentVertex
            && !isError)
        {
            // NOTE Check for entering cycles
            // We check if we have been on this vertex before
            for(j=0; j<manyFirst; ++j)
                if(currentVertex == first[j]) {
                    if(!isFirstRound[j]) {
                        isError = true;
                        printf("ERROR %d\n", i); //TODO Logging Debugging
                        break;
                    }
                }
        if(!isError)
        {
            float angle = angle_between_ABC(currentVertex->value, currentVertex->next->value, currentVertex->next->next->value);
            assert(angle<TAU && angle >=0);

            if(angle < TAU/2) { //Check for concavity
                // TODO Check if other vertices fall inside this triangle

                // Convex angle, triangulate
                Triangle t = ZERO_STRUCT;
                t.vertices[0] = currentVertex->value;
                t.vertices[1] = currentVertex->next->value;
                t.vertices[2] = currentVertex->next->next->value;
                outData[dataI++] = t;

                // Note: go on skupping ear
                currentVertex->next = currentVertex->next->next;
            }
            else {
                // NOTE: This triangle would not be correct, go on
                currentVertex = currentVertex->next;
                isFirstRound[manyFirst++] = false;
            }
        }
        }

    }
    TriangleList tl = create_triangleList(program, outData);
    tl.triangles_length = dataI;
    triangleList_update_pos(tl, d2);
    return tl;
}

void triangleList_update_pos(TriangleList tl, Data d) {
    float* data = (float*)d;

    int i=0, dataI=0;
    for(i=0, dataI=0; i<tl.triangles_length; ++i) {
        Triangle t = tl.triangles[i];
        int vertex; int dimension;
        for(vertex=0; vertex<3; ++vertex) {
            for(dimension=0; dimension<2; ++dimension)
                data[dataI++] = t.vertices[vertex].els[dimension];
        }
    }
    assert(tl.triangles_length*3*2 == dataI);
    glUseProgram(tl.program);
    GLint pos = glGetAttribLocation(tl.program, "position");
    u8 stride = (2)*sizeof(float);

    glBindBuffer(GL_ARRAY_BUFFER, tl.pos_buffer);
    glBufferData(GL_ARRAY_BUFFER, dataI*sizeof(float), data, GL_STATIC_DRAW);
    glVertexAttribPointer(pos, 2, GL_FLOAT, GL_FALSE, stride, 0);
}

void render_triangleList(TriangleList tl, Data* unis_untyped, s32 unis_length) {
    UniformData* unis = (UniformData*) unis_untyped;

    glUseProgram(tl.program);
    int i;
    for(i=0; i<unis_length; i++){
        setParam(tl.program, unis[i]);
    }
    GLint pos = glGetAttribLocation(tl.program, "position");
    glBindBuffer(GL_ARRAY_BUFFER, tl.pos_buffer);
    glVertexAttribPointer(pos, 2, GL_FLOAT, GL_FALSE, 2*sizeof(float), 0);
    glEnableVertexAttribArray(pos);

    glDrawArrays(GL_TRIANGLES, 0, tl.triangles_length*3);

}
