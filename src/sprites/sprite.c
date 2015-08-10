#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include <GL/glew.h>
#ifdef JS
#include <emscripten.h>
#endif

#include "../puun/puun.h"
#include "../puun/gf/sprite.h"

#ifndef ASSETSPATH
#define ASSETSPATH(name) "../assets/" # name
#endif

#define TAU 6.2832

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
Square* paddle;
Square* ball;
SpriteList Sprites;
SpriteSheet* sheet;

u8 program;

static GLuint ImageId;
static u8* Image;

static Data buffer;

void init() {
    running = true;
    buffer = malloc(4000);
    Data d = malloc(4*sizeof(Square));
    sheet = loadSpriteSheet(ASSETSPATH(/atlas.png));
    Sprites = spritesFromSheet(*sheet, d);

    glEnable(GL_BLEND);
    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDepthMask(1);


    Square s = {0};
    s.width = 0.8; s.height = 0.1;
    s.u1 = 0./512; s.u2= 256./512;
    s.v1 = 0./256; s.v2 = 49./256;
    s.position.z = +0.8;
    Sprites.squareList.squares[0] = s;
    s.width = 0.2; s.height = 0.2;
    s.u1 = 256./512; s.u2= 384./512;
    s.v1 = 0./256; s.v2 = 128./256;
    s.position.z = 0;
    Sprites.squareList.squares[1] = s;
    Sprites.squareList.squares_length = 2;

    paddle = &Sprites.squareList.squares[0];
    ball = &Sprites.squareList.squares[1];

}

float s2p(int screen) {
    screen -= 400;
    float s = screen;
    s /= 400;
    return s;
}

void bounce(float wallDegree, float* vx, float* vy, float* av, float incVel) {
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
    float a, b;
    float time;
    u32 ms;
    getTimeElapsed(&ms);
    time = ms / 1000.f;
    getMousePosition(&a, &b);
    paddleX = s2p(a);
    paddleRot = s2p(b);
    static float rotate = 0;
    static float vx = 0, vy = -1, px = 0, py = 0, av =0;
    static char hasBounced = 0;

    rotate += av*time;
    px += time*vx, py += time*vy;
    square_rotateTo(ball, rotate);
    square_traslateTo(ball, px, py);

    square_traslateTo(paddle, paddleX, paddleY);
    square_rotateTo(paddle, paddleRot);

    squareList_update_pos(Sprites.squareList, buffer);

    //CHECK COLLISION :)
    if(hasBounced!= 1 && px <-1){ bounce(TAU/4, &vx, &vy, &av, 1.);hasBounced = 1; }
    if(hasBounced!=2 && px> 1){ bounce(-TAU/4, &vx, &vy, &av, 1.);hasBounced = 2;}
    if(hasBounced!=3 && py >1){ bounce(-TAU/2, &vx, &vy, &av, 1.); hasBounced = 3; }
    if(py <-1){
        die();
    }
    if(hasBounced!=4 && px<= paddleX+0.4 && px>=paddleX-0.4  && py<=paddleY+0.05 && py>=paddleY-0.05) {
        bounce(3/4*TAU-paddleRot, &vx, &vy, &av, 1.2);
        hasBounced = 4;
        score++;
    }
    puun_MouseClick click;
    getMouseClick(&click);
    puun_KEY key;
    getKeyboardKey(&key);
    if(click)
        printf("MOUSE CLICKED\n", 0);
    if(key.isPressed) printf("%c\n", key.key);




    glClearColor(1.0, 1.0, 1.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

    renderSpriteList(Sprites);
    //uniImg.id = ballId;
    //uniImg.texnum = 0;
    //render_squareList(Squares, (Data)&uniImg, 1);


    puun_SWAP_BUFFERS();
    //SDL_GL_SwapBuffers();
}

void die() {
    //free(buffer);
    running = false;
    printf("YOUR SCORE IS %d\n", score);
 //   sdl_die();
}
