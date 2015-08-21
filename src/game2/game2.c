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

#ifndef ASSETSPATH
#define ASSETSPATH(name) "../assets/" # name
#endif

#define TAU 6.28318530717958647692

static u32 polygonProgam;

void init() {
    running = true;
    Square buu = create_square(120, 60);
    char vs[] = "attribute vec2 position; attribute vec2 uv;\n\
                 varying vec2 texcoord; void main(){\n\
                     gl_Position=vec4(position, 0, 1);\n\
                         texcoord=uv;}";
    char fs[] = "precision mediump float; \n\
                 uniform sampler2D texture; \n\
                 varying vec2 texcoord; \n\
                 void main() {\n\
                     gl_FragColor = texture2D(texture, texcoord);\n\
                 }";

    u32 program = setupProgram(vs, 0, fs, 0);
    SquareList sl = create_square_list(program, malloc(100*sizeof(Square)));

    char vs2[] = "attribute vec2 position;\n\
          void main(){\n\
             gl_Position=vec4(position, 0, 1);\n\
          }";
    char fs2[] = "precision mediump float; \n\
          uniform vec4 colour;\n\
          void main() {\n\
            gl_FragColor = colour;\n\
          }";
    polygonProgam = setupProgram(vs2, 0, fs2, 0);

    assert(sl.squares);
    assert(polygonProgam);
    assert(buu.width);

}

void render_polygon(float* vertices, int verticesCount) {

}

union v2 {
    struct {
        float x, y;
    };
    float els[2];
};

typedef union v2 v2;

struct game_ship {
    v2 vertices [10];
    v2 pos;
};

typedef struct game_ship Game_Ship;

void updateNrender() {
    puun_KEY character;
    getKeyboardKey(&character);
    if(character.isPressed) {
        bool isF4 = (character.key == 115) && (character.alt & puun_ALT);
        running = !isF4;
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

    glClearColor(1,1,1,1);
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    puun_SWAP_BUFFERS();
}

void die() {
    running = false;
}
