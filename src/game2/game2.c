#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>
#include <assert.h>
#include <GL/glew.h>

#include "../puun/puun.h"
#include "../puun/types.h"
#include "../puun/gf/sprite.h"
#include "../puun/gf/text.h"
#include "../puun/gf/gl_help.h"

#include "polygon.c"


// MACROS THAT PERHAPS SHOULD BE OTHERWHERE

#ifndef ASSETSPATH
#define ASSETSPATH(name) "../assets/" # name
#endif

#define TAU 6.28318530717958647692

#define ARRCOUNT(x) (sizeof((x))/sizeof((x)[0]))

struct Game_Memory {
    v2 hexagon[6];
    u32 program;
    u32 vertexBuffer;
    u32 elementBuffer;
      Polygon ps[10];
      PolygonList pl;
      v2 polygonData[13][10];
      u8 Buffer[2000];
      u8 Buffer2[2000];
};

const u32 hexagonDrawOrder[] = {0, 1, 2,
                                0, 2, 3,
                                0, 3, 4,
                                0, 4, 5};

void init(Data game_memory) {
    struct Game_Memory* mem = (struct Game_Memory*)game_memory;

    u8 vs[] = "attribute vec2 position;\n\
                 void main(){\n\
                     gl_Position=vec4(position, 0, 1);\n\
                         }";
    u8 fs[] = "precision mediump float; \n\
               uniform vec4 colour;\n\
                 void main() {\n\
                     gl_FragColor = colour;//texture2D(texture, texcoord);\n\
                 }";

    mem->program = setupProgram(vs, 0, fs, 0);

    mem->pl = create_polygonList(mem->program, mem->ps);
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
    for(i=0, j=0; i<6; i++) {
        Polygon* p = &mem->ps[i];
        p->value = mem->polygonData[i];
        p->num = i+3;
        v2 base = {};
        base.x = 0.8*cos(i*TAU/6.f);
        base.y = 0.8*sin(i*TAU/6.f);
        for(j=0; j<p->num; j++) {
            v2 a = {};
            a.x = 0.2*cos(j*TAU/p->num);
            a.y = 0.2*sin(j*TAU/p->num);
            a = add_v2(a, base);
            p->value[j] = a;
        }
    }
    mem->pl.polygons_length = 6;
    polygonList_update_pos(&mem->pl, mem->Buffer, mem->Buffer2);
#endif
#if 1
    glUseProgram(mem->program);
    GLint pos = glGetAttribLocation(mem->program, "position");
    glGenBuffers(1, &mem->vertexBuffer);
    glGenBuffers(1, &mem->elementBuffer);

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
    if(character.isPressed) {
        bool isF4 = (character.key == 29) && (character.alt & puun_ALT);
        if(isF4) {
            game_die(game_memory);
            return;
        }
        bool isF5 = (character.key == 30);
        if(isF5) {
            init(game_memory);
            return;
        }
    }

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

    glUniform4f(glGetUniformLocation(mem->program, "colour"), 0.5, 0.75, 0.25, 1.0);

    GLint pos = glGetAttribLocation(mem->program, "position");
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mem->elementBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, mem->vertexBuffer);
    glVertexAttribPointer(pos, 2, GL_FLOAT, GL_FALSE, 2*sizeof(float), 0);
    glEnableVertexAttribArray(pos);
    glDrawElements(GL_TRIANGLES, ARRCOUNT(hexagonDrawOrder), GL_UNSIGNED_INT, (void*)0);


    glUniform4f(glGetUniformLocation(mem->program, "colour"), 0.75, 0.25, 0.5, 1.0);
    render_polygonList(mem->pl, 0, 0);
    puun_SWAP_BUFFERS();
}

void game_die() {
    platform_die();
}
