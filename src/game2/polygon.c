#include "GL/glew.h"
#include <assert.h>
#include <string.h>

#include "../puun/gf/gl_help.h"

#include "polygon.h"

b32 intersect_polygon(Polygon a, Polygon b) {
//TODO Implement
    return false;
}


PolygonList create_polygonList(u8 program, Polygon* polygons) {
    PolygonList pl = {};
    pl.polygons = polygons;

    pl.polygons_length = 0;
    pl.program = program;
    glGenBuffers(1, &pl.pos_buffer);
    glGenBuffers(1, &pl.els_buffer);

    return pl;
}

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
    TriangleList tl = {};

    tl.triangles_length = 0;
    tl.triangles = triangles;
    tl.program = program;
    glGenBuffers(1, &tl.pos_buffer);

    return tl;
}

typedef DOUBLE_LINKED_LIST(Vertex) Vertex_Doubled_Link;

TriangleList triange_list_from_polygon(u8 program, Polygon* ps, s32 polygon_count, Data d, Data d2) {
    Triangle* outData = (Triangle*)d;

    int i, dataI;
    for(i=0, dataI=0; i<polygon_count; ++i)
    {
        Polygon p = ps[i]; 
        Vertex_Doubled_Link verts[p.num];
        int j;
        for(j=0; j<p.num; ++j) {
            Vertex_Doubled_Link* v = &verts[j]; 
            v->value = p.value[j];
            v->next = verts + ((j + 1) % p.num);            
            v->previous = verts + ((j +p.num - 1) % p.num);            
        }
        Vertex_Doubled_Link* currentVertex = &verts[0];
        b32 isError = false;;
        Vertex_Doubled_Link* first[p.num];
        first[0] = currentVertex;
        s32 manyFirst = 1;
        b32 isFirstRound[p.num];
        for(j=0; j<p.num; ++j) {
            isFirstRound[j] = true;
        }
        while(currentVertex->next != currentVertex 
            && currentVertex->next->next != currentVertex
            && !isError)
        {
            for(j=0; j<manyFirst; ++j)
                if(currentVertex == first[j]) {
                    if(!isFirstRound[j]) {
                        isError = true;
                        printf("ERROR %d\n", i);
                        break;
                    }
                }
        if(!isError)
        {
           // if(!currentVertex->next->next == currentVertex) {

          //  v2 toPrevious = sub_v2(currentVertex->previous->value, currentVertex->value);
          //v2 toNext = sub_v2(currentVertex->next->value, currentVertex->value);
          //v2 toNextFromNextNext = sub_v2(currentVertex->next->value, currentVertex->next->next->value);
            #if 0
            float angle = angle_between_v2(toNext,toPrevious);   
            float angle2 = angle_between_v2(toNext,toNextNext);   

            if(angle > TAU/2) {
                assert(0 > 0); //THERES PROBLEM 
            }
            if(angle2 > TAU/2) {
                assert(0); //THERES PROBLEM 
            }
            #endif
            float angle = angle_between_ABC(currentVertex->value, currentVertex->next->value, currentVertex->next->next->value);   
            assert(angle<TAU && angle >=0);
            //angle = TAU - angle;
            if(angle < TAU/2) {
                //printf("i:%d angle:%f angle2:%f\n", i, 360/TAU*angle, 360/TAU*angle2);
                Triangle t = {};
                t.vertices[0] = currentVertex->value;
                t.vertices[1] = currentVertex->next->value;
                t.vertices[2] = currentVertex->next->next->value;
                outData[dataI++] = t;

                currentVertex->next = currentVertex->next->next;
            }
            else {
            //    printf("Stuff i:%d\n", i);
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
