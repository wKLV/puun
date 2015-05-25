#include "square.h"
#include "gl_help.c"
#include <math.h>

Square create_square(float width, float height) {
    Square square = {};
    square.height = height;
    square.width = width;

    return square;
}
void square_traslate(Square* square, float x, float y) {
    square->x = x;
    square->y = y;
}
void square_rotate(Square* square, float angle) {
    square->rotation = angle;
}


SquareList create_square_list(u8 program, Data squares) {
    SquareList sl = {};
    sl.squares = squares;
    sl.squares_length = 0;

    sl.program = program;
    glGenBuffers(1, &sl.geo_buffer);
    glGenBuffers(1, &sl.pos_buffer);
    glGenBuffers(1, &sl.elem_buffer);

//    GLushort elements[] = {0,1,2,3};
//    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sl.elem_buffer);
//    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elements), elements, GL_STATIC_DRAW);
    return sl;
}


void squareList_update_pos(SquareList sl, Data d) {
    float* data = (float*)d;

    int i, dataI;
    for(i=0, dataI=0; i<sl.squares_length; i++) {
        Square s = sl.squares[i];
        float lx = -s.width/2, by = -s.height/2,
              rx = s.width/2, ty = s.height/2,
              co = cos(s.rotation), si = sin(s.rotation);
        float blx =   co*lx + si*by + s.x;
        float bly = - si*lx + co*by + s.y;
        float brx =   co*rx + si*by + s.x;
        float bry = - si*rx + co*by + s.y;
        float tlx =   co*lx + si*ty + s.x;
        float tly = - si*lx + co*ty + s.y;
        float trx =   co*rx + si*ty + s.x;
        float try = - si*rx + co*ty + s.y;
        { // Vertex Bottom Left
            //pos
            data[dataI] = blx; dataI++;
            data[dataI] = bly; dataI++;
            data[dataI] = 0.; dataI++;

            //uv
            data[dataI] = s.u1; dataI++;
            data[dataI] = s.v2; dataI++;

            //scale
            data[dataI] = 1.; dataI++;
            data[dataI] = 1.; dataI++;
        }
        { // Vertex Bottom Right
            //pos
            data[dataI] = brx; dataI++;
            data[dataI] = bry; dataI++;
            data[dataI] = s.z; dataI++;

            //uv
            data[dataI] = s.u2; dataI++;
            data[dataI] = s.v2; dataI++;

            //scale
            data[dataI] = 1.; dataI++;
            data[dataI] = 1.; dataI++;
        }
        { // Vertex Top Left
            //pos
            data[dataI] = tlx; dataI++;
            data[dataI] = tly; dataI++;
            data[dataI] = s.z; dataI++;

            //uv
            data[dataI] = s.u1; dataI++;
            data[dataI] = s.v1; dataI++;

            //scale
            data[dataI] = 1.; dataI++;
            data[dataI] = 1.; dataI++;
        }
        { // Vertex Bottom Right
            //pos
            data[dataI] = brx; dataI++;
            data[dataI] = bry; dataI++;
            data[dataI] = s.z; dataI++;

            //uv
            data[dataI] = s.u2; dataI++;
            data[dataI] = s.v2; dataI++;

            //scale
            data[dataI] = 1.; dataI++;
            data[dataI] = 1.; dataI++;
        }
        { // Vertex Top Left
            //pos
            data[dataI] = tlx; dataI++;
            data[dataI] = tly; dataI++;
            data[dataI] = s.z; dataI++;

            //uv
            data[dataI] = s.u1; dataI++;
            data[dataI] = s.v1; dataI++;

            //scale
            data[dataI] = 1.; dataI++;
            data[dataI] = 1.; dataI++;
        }
        { // Vertex Top Right
            //pos
            data[dataI] = trx; dataI++;
            data[dataI] = try; dataI++;
            data[dataI] = s.z; dataI++;

            //uv
            data[dataI] = s.u2; dataI++;
            data[dataI] = s.v1; dataI++;

            //scale
            data[dataI] = 1.; dataI++;
            data[dataI] = 1.; dataI++;
        }
    }

    GLint pos = glGetAttribLocation(sl.program, "position");
    GLint uv = glGetAttribLocation(sl.program, "uv");
    GLint scale = glGetAttribLocation(sl.program, "scale");

    u8 stride = (3+2+2)*sizeof(float);

    glBindBuffer(GL_ARRAY_BUFFER, sl.pos_buffer);
    glBufferData(GL_ARRAY_BUFFER, sl.squares_length*6*(3+2+2)*sizeof(float),
            data, GL_DYNAMIC_DRAW);
    glVertexAttribPointer(pos, 3, GL_FLOAT, GL_FALSE,
            stride, 0);
    glEnableVertexAttribArray(pos);
    glVertexAttribPointer(uv, 2, GL_FLOAT, GL_FALSE,
            stride, (Data)(3*sizeof(float)));
    glEnableVertexAttribArray(uv);
    glVertexAttribPointer(scale, 2, GL_FLOAT, GL_FALSE,
            stride, (Data)(5*sizeof(float)));
    glEnableVertexAttribArray(scale);
}

void render_squareList(SquareList sl, Data* unis_untyped,
        s32 unis_length) {
    UniformData* unis = (UniformData*) unis_untyped;

    glUseProgram(sl.program);
    int i;
    for(i=0; i<unis_length; i++){
        setParam(sl.program, unis[i]);
    }

    glDrawArrays(GL_TRIANGLES, 0, 6*sl.squares_length);
//    glDrawElements(GL_TRIANGLE_STRIP, 4,
//            GL_UNSIGNED_SHORT, (void*)0);
}
