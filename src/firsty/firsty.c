#include <stdio.h>
#include <stdbool.h>
#include <math.h>

#include "../puun/puun.h"
#include "../puun/types.h"
#include "../puun/gf/square.c"
#include "../other/stb_image.h"

#define TAU 6.2832

#ifndef ASSETSPATH
#define ASSETSPATH(name) "../assets/" # name
#endif

void scaleMatrix(float a, float* matrix) {
    matrix[0] *= a;
    matrix[1] *= a;
    matrix[3] *= a;
    matrix[4] *= a;
}

void traslateMatrix(float x, float y, float* matrix) {
    matrix[6] = x;
    matrix[7] = y;
}

void rotateMatrix(float a, float* matrix) {
    float c = cos(a), s = sin(a);
    matrix[0] = c;
    matrix[1] = -s;
    matrix[3] = s;
    matrix[4] = c;
}

struct Game_Memory {
    u32 vertexBuffer;
    u32 elementBuffer;
    u32 program;
    float paddleMatrix[9];
    float ballMatrix[9];
    u32 paddleId;
    u32 ballId;
    float rotate;
    float vx;
    float vy;
    float px;
    float py;
    float av;
    u32 score;
    b32 hasBounced;
};

void init(Data game_memory) {
    u32 ms;
    getTimeElapsed(&ms);
    square_init();
        u8 vertexSource[] = "precision mediump float; attribute vec2 position;\n\
                               uniform mat3 world;\n\
                               uniform float depth; \n\
                               varying vec2 texcoord;\n\
                               void main() {\n\
                                   gl_Position = vec4(world*(vec3(position, 1))+vec3(0,0,depth-1.), 1.0);\n\
                                   texcoord = position*vec2(0.5) +vec2(0.5);\n\
                               }";
        u8 fragmentSource[] = "precision mediump float; \n\
                                 uniform sampler2D texture; \n\
                                varying vec2 texcoord; \n\
                               void main() {\n\
                               gl_FragColor = texture2D(texture, texcoord);\n\
                               }\n\
                              ";
    struct Game_Memory* mem = (struct Game_Memory*) game_memory;
    mem->program = square_createType(vertexSource, sizeof(vertexSource), fragmentSource, sizeof(fragmentSource));
    int x, y, n;
    u8* ballImage = stbi_load(ASSETSPATH(/ball.png), &x, &y, &n, 0);
    mem->ballId = setupTexture(ballImage, x, y);
    u8* paddleImage = stbi_load(ASSETSPATH(/paddle.png), &x, &y, &n, 0);
    mem->paddleId = setupTexture(paddleImage, x, y);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDepthMask(1);

    scaleMatrix(0.2, mem->paddleMatrix);
    loadSoundFile(ASSETSPATH(bout.wav));
    loadSoundFile(ASSETSPATH(ancient.ogg));

    mem->vy = -1;
}

float s2p(int screen) {
    screen -= 400;
    float s = screen;
    s /= 400;
    return s;
}

void bounce(float wallDegree, float* vx, float* vy, float* av, float incVel) {
    playSound(0);
    float in = atan2(*vy, *vx);
    float vel = sqrt(*vx* *vx + *vy* *vy);
    float n = wallDegree;
    float out = n + n - in;
    float s = sin(out), c = cos(out);
    float vvx = vel*c;
    float vvy = vel*s;
    *vx = vvx*incVel, *vy = vvy*incVel;
}
    float paddleX =0, paddleY=-.8, paddleRot;

void updateMouse(int x, int y) {
    paddleX = s2p(x);
    paddleRot = s2p(y);
}

void updateNrender(Data game_memory){
    struct Game_Memory* mem = (struct Game_Memory*) game_memory;

    puun_KEY key;
    getKeyboardKey(&key);
    float x = 400, y = 400;
    float time;
    u32 ms;
    getMousePosition(&x, &y);
    getTimeElapsed(&ms);
    time = ms /1000.f;
    paddleX = s2p(x);
    paddleRot = s2p(y);
    if(key.isPressed && key.key == 'p')
        playSound(1);

    mem->rotate += mem->av*1*time;
    mem->px += 1*mem->vx*time, mem->py += 1*mem->vy*time;
    rotateMatrix(mem->rotate, mem->ballMatrix);
    scaleMatrix(0.08, mem->ballMatrix);
    traslateMatrix(mem->px, mem->py, mem->ballMatrix);

    traslateMatrix(paddleX, paddleY, mem->paddleMatrix);
    rotateMatrix(paddleRot, mem->paddleMatrix);
    scaleMatrix(0.2, mem->paddleMatrix);

    //CHECK COLLISION :)
    if(mem->hasBounced!= 1 && mem->px <-1){ bounce(TAU/4, &mem->vx, &mem->vy, &mem->av, 1.);mem->hasBounced = 1; }
    if(mem->hasBounced!=2 && mem->px> 1){ bounce(-TAU/4, &mem->vx, &mem->vy, &mem->av, 1.);mem->hasBounced = 2;}
    if(mem->hasBounced!=3 && mem->py >1){ bounce(-TAU/2, &mem->vx, &mem->vy, &mem->av, 1.); mem->hasBounced = 3; }
    if(mem->py <-1){
        game_die(game_memory); return;
    }
    if(mem->hasBounced!=4) if(mem->px<= paddleX+0.3 && mem->px>=paddleX-0.2  && mem->py<=paddleY+0.3 && mem->py>=paddleY-0.2) {
        bounce(3/4*TAU-paddleRot, &mem->vx, &mem->vy, &mem->av, 1.2);
        mem->hasBounced = 4;
        mem->score++;
    }
    glClearColor(1.0, 1.0, 1.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

    UniformData data[3] = {};

    UniformData uni = {};
    uni.name = (u8*)"world";
    uni.dataStructure = M3;
    uni.dataType = GL_FLOAT;
    uni.data = mem->paddleMatrix;

    UniformData uniImg = {};
    uniImg.name = (u8*)"texture";
    uniImg.dataStructure = Texture;
    uniImg.texnum = 0;
    uniImg.id = mem->paddleId;

    float depth = 0.9;
    data[2].name = (u8*)"depth";
    data[2].dataStructure = One;
    data[2].dataType = GL_FLOAT;
    data[2].data = &depth;

    data[0] = uni;
    data[1] = uniImg;

    square_render(mem->program, 4, data, 3);
    data[0].data = mem->ballMatrix;
    depth=0;
    data[1].id = mem->ballId;
    data[1].texnum = 0;
    square_render(mem->program, 4, data, 3);

    puun_SWAP_BUFFERS();
}

void game_die(Data game_memory) {
    struct Game_Memory* mem = (struct Game_Memory*) game_memory;
    printf("YOUR SCORE IS %d\n", mem->score);
    platform_die();
}
