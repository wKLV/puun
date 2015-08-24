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

static GLuint vertexBuffer,
       elementBuffer,
       program;
static GLfloat paddleMatrix[] = {
    1.0, 0.0, 0.0,
    0.0, 1.0, 0.0,
    0.0, 0.0, 1.0
};

static GLfloat ballMatrix[] = {
    1.0, 0.0, 0.0,
    0.0, 1.0, 0.0,
    0.0, 0.0, 1.0
};
static GLuint paddleId;
static GLuint ballId;
static u8* paddleImage;
static u8* ballImage;

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

void init() {
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
    program = square_createType(vertexSource, sizeof(vertexSource), fragmentSource, sizeof(fragmentSource));
    int x, y, n;
    ballImage = stbi_load(ASSETSPATH(/ball.png), &x, &y, &n, 0);
    ballId = setupTexture(ballImage, x, y);
    paddleImage = stbi_load(ASSETSPATH(/paddle.png), &x, &y, &n, 0);
    paddleId = setupTexture(paddleImage, x, y);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDepthMask(1);

    scaleMatrix(0.2, paddleMatrix);
    loadSoundFile(ASSETSPATH(bout.wav));
    loadSoundFile(ASSETSPATH(ancient.ogg));
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

static int score = 0;
static float  paddleX =0, paddleY=-.8, paddleRot;

void updateMouse(int x, int y) {
    paddleX = s2p(x);
    paddleRot = s2p(y);
}

void updateNrender(){
    puun_KEY key;
    getKeyboardKey(&key);
    float x, y;
    float time;
    u32 ms;
    getMousePosition(&x, &y);
    getTimeElapsed(&ms);
    time = ms /1000.f;
    paddleX = s2p(x);
    paddleRot = s2p(y);
    if(key.isPressed && key.key == 'p')
        playSound(1);
    static float rotate = 0;
    static float vx = 0, vy = -1, px = 0, py = 0, av =0;
    static char hasBounced = 0;

    rotate += av*1*time;
    px += 1*vx*time, py += 1*vy*time;
    rotateMatrix(rotate, ballMatrix);
    scaleMatrix(0.08, ballMatrix);
    traslateMatrix(px, py, ballMatrix);

    traslateMatrix(paddleX, paddleY, paddleMatrix);
    rotateMatrix(paddleRot, paddleMatrix);
    scaleMatrix(0.2, paddleMatrix);

    //CHECK COLLISION :)
    if(hasBounced!= 1 && px <-1){ bounce(TAU/4, &vx, &vy, &av, 1.);hasBounced = 1; }
    if(hasBounced!=2 && px> 1){ bounce(-TAU/4, &vx, &vy, &av, 1.);hasBounced = 2;}
    if(hasBounced!=3 && py >1){ bounce(-TAU/2, &vx, &vy, &av, 1.); hasBounced = 3; }
    if(py <-1){
        game_die(); return;
    }
    if(hasBounced!=4 && px<= paddleX+0.3 && px>=paddleX-0.2  && py<=paddleY+0.3 && py>=paddleY-0.2) {
        bounce(3/4*TAU-paddleRot, &vx, &vy, &av, 1.2);
        hasBounced = 4;
        score++;
    }
    glClearColor(1.0, 1.0, 1.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

    UniformData data[3] = {};

    UniformData uni = {};
    uni.name = (u8*)"world";
    uni.dataStructure = M3;
    uni.dataType = GL_FLOAT;
    uni.data = paddleMatrix;

    UniformData uniImg = {};
    uniImg.name = (u8*)"texture";
    uniImg.dataStructure = Texture;
    uniImg.texnum = 0;
    uniImg.id = paddleId;

    float depth = 0.9;
    data[2].name = (u8*)"depth";
    data[2].dataStructure = One;
    data[2].dataType = GL_FLOAT;
    data[2].data = &depth;

    data[0] = uni;
    data[1] = uniImg;

    square_render(program, 4, data, 3);
    data[0].data = ballMatrix;
    depth=0;
    data[1].id = ballId;
    data[1].texnum = 0;
    square_render(program, 4, data, 3);

    puun_SWAP_BUFFERS();
}

void game_die() {
    printf("YOUR SCORE IS %d\n", score);
    platform_die();
}
