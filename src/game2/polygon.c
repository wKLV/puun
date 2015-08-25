#include "GL/glew.h"

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
