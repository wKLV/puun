#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>
#include <assert.h>
#include <GL/glew.h>
#include <SDL/SDL.h>

#include "../puun/puun.h"
#include "../puun/types.h"
#include "../puun/gf/sprite.h"
#include "../puun/gf/text.h"
#include "../puun/gf/gl_help.h"

#define puun_2D 1
#include "polygon.c"


/*
 * MACROS THAT PERHAPS SHOULD BE OTHERWHERE
 */

#ifndef ASSETSPATH
#define ASSETSPATH(name) "../assets/" # name
#endif


struct Game_Memory {
    u32 program;
    u32 vertexBuffer;
    u32 elementBuffer;
    b32 isKeyPressed;
    v2 hexagon[6];
    Polygon ps[7];
    PolygonList pl;
    TriangleList tl;
    v2 polygonData[7][20];
    u8 Buffer[2000];
    u8 Buffer2[8000];
};

const u32 hexagonDrawOrder[] = {0, 1, 2,
                                0, 2, 3,
                                0, 3, 4,
                                0, 4, 5};
void init_geometry(Data game_memory);
void init(Data game_memory) {
    struct Game_Memory* mem = (struct Game_Memory*)game_memory;

    u8 vs[] = "attribute vec2 position;\n\
                 void main(){\n\
                     gl_Position=vec4(position, 0, 1);\n\
                         }";
    u8 fs[] = "precision mediump float; \n\
               uniform vec4 colour;\n\
               varying float num;\n\
                 void main() {\n\
                    gl_FragColor = colour;//texture2D(texture, texcoord);\n\
                 }";

    mem->program = setupProgram(vs, 0, fs, 0);

  //  mem->pl = create_polygonList(mem->program, mem->ps);
    glGenBuffers(1, &mem->vertexBuffer);
    glGenBuffers(1, &mem->elementBuffer);
    init_geometry(game_memory);
}
void init_geometry(Data game_memory) {
    struct Game_Memory* mem = (struct Game_Memory*)game_memory;
    int i;
#if 1
    for(i=0;i<6;i++) {
        v2 a = {};
        a.x = cos(i*TAU/6.f);
        a.y = sin(i*TAU/6.f);
        mem->hexagon[i] = a;
    }
#endif
#if 1
    int j;
    for(i=0; i<7; i++) {
        Polygon* p = &mem->ps[i];
        p->value = mem->polygonData[i];
    }
    for(i=2, j=0; i<7; i++) {
        Polygon* p = &mem->ps[i];
        p->num = 12;
        v2 base = {};
        base.x = 0.7*cos((i-1)*TAU/6.f);
        base.y = 0.7*sin((i-1)*TAU/6.f);
        for(j=0; j<p->num; j++) {
            v2 a = {};
            a.x = 0.2*cos(j*TAU/p->num);
            a.y = 0.2*sin(j*TAU/p->num);
            if(i%3!=0){ if(j%3 == 0)
                a = scalar_v2(a, 0.5);
            }
            a = add_v2(a, base);
            p->value[j] = a;
        }
    }
    Polygon* p = &mem->ps[0];
    p->num = 12;
    p->value[0] = new_v2(0.3, 0.1);
    p->value[1] = new_v2(0.1, 0.1);
    p->value[2] = new_v2(0.1, 0.3);
    p->value[3] = new_v2(-0.1, 0.3);
    p->value[4] = new_v2(-0.1, 0.1);
    p->value[5] = new_v2(-0.3, 0.1);
    p->value[6] = new_v2(-0.3, -0.1);
    p->value[7] = new_v2(-0.1, -0.1);
    p->value[8] = new_v2(-0.1, -0.3);
    p->value[9] = new_v2(0.1, -0.3);
    p->value[10] = new_v2(0.1, -0.1);
    p->value[11] = new_v2(0.3, -0.1);

    p = &mem->ps[1];  
    p->num = 20;
    v2 base = {};
    base.x = 0.7;
    base.y = 0;
    float innerWidth = 0.15;
    float outerWidth = 0.1;
    float innerIntersecion = 0.05;
    p->value[0] = add_v2(base, new_v2(innerWidth+outerWidth, innerWidth-innerIntersecion));
    p->value[1] = add_v2(base, new_v2(innerWidth+outerWidth, innerWidth+outerWidth));
    p->value[2] = add_v2(base, new_v2(innerWidth-innerIntersecion, innerWidth+outerWidth));
    p->value[3] = add_v2(base, new_v2(innerWidth-innerIntersecion, innerWidth));
    p->value[4] = add_v2(base, new_v2(-innerWidth+innerIntersecion, innerWidth));
    p->value[5] = add_v2(base, new_v2(-innerWidth+innerIntersecion, innerWidth+outerWidth));
    p->value[6] = add_v2(base, new_v2(-innerWidth-outerWidth, innerWidth+outerWidth));
    p->value[7] = add_v2(base, new_v2(-innerWidth-outerWidth, innerWidth-innerIntersecion));
    p->value[8] = add_v2(base, new_v2(-innerWidth, innerWidth-innerIntersecion));
    p->value[9] = add_v2(base, new_v2(-innerWidth, -innerWidth+innerIntersecion));
    p->value[10] = add_v2(base, new_v2(-innerWidth-outerWidth, -innerWidth+innerIntersecion));
    p->value[11] = add_v2(base, new_v2(-innerWidth-outerWidth, -innerWidth-outerWidth));
    p->value[12] = add_v2(base, new_v2(-innerWidth+innerIntersecion, -innerWidth-outerWidth));
    p->value[13] = add_v2(base, new_v2(-innerWidth+innerIntersecion, -innerWidth));
    p->value[14] = add_v2(base, new_v2(innerWidth-innerIntersecion, -innerWidth));
    p->value[15] = add_v2(base, new_v2(innerWidth-innerIntersecion, -innerWidth-outerWidth));
    p->value[16] = add_v2(base, new_v2(innerWidth+outerWidth, -innerWidth-outerWidth));
    p->value[17] = add_v2(base, new_v2(innerWidth+outerWidth, -innerWidth+innerIntersecion));
    p->value[18] = add_v2(base, new_v2(innerWidth, -innerWidth+innerIntersecion));
    p->value[19] = add_v2(base, new_v2(innerWidth, +innerWidth-innerIntersecion));

    #if 0
    base.x = 0.7*cos(2*TAU/6.f);
    base.y = 0.7*sin(2*TAU/6.f);
    p = &mem->ps[2];
    p->value[1] = add_v2(base, new_v2(0.1, 0.1));
    p->value[0] = add_v2(base, new_v2(0, 0));
    #endif

    mem->tl = triange_list_from_polygon(mem->program, mem->ps, 7, mem->Buffer, mem->Buffer2);
  //    mem->pl.polygons_length = 6;
  //    polygonList_update_pos(&mem->pl, mem->Buffer, mem->Buffer2);
#endif
#if 1
    glUseProgram(mem->program);
    GLint pos = glGetAttribLocation(mem->program, "position");

    glBindBuffer(GL_ARRAY_BUFFER, mem->vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(mem->hexagon), mem->hexagon, GL_STATIC_DRAW);
    glVertexAttribPointer(pos, 2, GL_FLOAT, GL_FALSE, 2*sizeof(float), 0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mem->elementBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(hexagonDrawOrder), hexagonDrawOrder, GL_STATIC_DRAW);
#endif

   // assert(sl.squares);
}

void updateNrender(Data game_memory) {
    struct Game_Memory* mem = (struct Game_Memory*)game_memory;


    puun_KEY character;
    getKeyboardKey(&character);
    if((!mem->isKeyPressed) && character.isPressed) {
        mem->isKeyPressed = true;
        bool isF4 = (character.key == 29) && (character.alt & puun_ALT);
        if(isF4) {
            game_die(game_memory);
            return;
        }
        bool isF5 = (character.key == 30);
        if(isF5) {
            memset(mem->hexagon, 0, sizeof(struct Game_Memory));
            init_geometry(game_memory);
            return;
        }
        bool isSpace = (character.key == SDLK_SPACE);
        if(isSpace) {
           init_geometry(game_memory);
           return;
        }
    }
    if(!character.isPressed) mem->isKeyPressed = false;

    //
    // ========================================
    //        THE AMAZING GAME OF LIFE & DEATH
    //
    //      Yeah, it basically just is there
    //             not doing anything
    //
    //   MASTER TODO:________________________________
    //      Game[ ]:
    //          Design[ ]: Ya tal
    //          Ships hanging around [ ]:
    //              a polygon?
    //          Player moving[ ]

    glClearColor(1.,1.,1.,1);
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

    glUseProgram(mem->program);

    glUniform4f(glGetUniformLocation(mem->program, "colour"), 0.5, 0.25, 0.75, 1.0);

    GLint pos = glGetAttribLocation(mem->program, "position");
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mem->elementBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, mem->vertexBuffer);
    glVertexAttribPointer(pos, 2, GL_FLOAT, GL_FALSE, 2*sizeof(float), 0);
    glEnableVertexAttribArray(pos);
    glDrawElements(GL_TRIANGLES, ARRCOUNT(hexagonDrawOrder), GL_UNSIGNED_INT, (void*)0);


    glUniform4f(glGetUniformLocation(mem->program, "colour"), 0.25, 0.5, 0.75, 1.0);
    //render_polygonList(mem->pl, 0, 0);
    render_triangleList(mem->tl, 0, 0);
    puun_SWAP_BUFFERS();
}

void game_die() {
    platform_die();
}
