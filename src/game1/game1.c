#include "GL/glew.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>

#include "../puun/puun.h"
#include "../puun/types.h"
#include "../puun/gf/sprite.h"
#include "../puun/gf/text.h"

#ifndef ASSETSPATH
#define ASSETSPATH(name) "../assets/" # name
#endif

#define TAU 6.28318530717958647692

float p2v(int pixels) {
    pixels -= 400;
    float s = pixels;
    s /= 400;
    return s;
}

float p2vs(int pixels) {
    return ((float)pixels)/400;
}

int v2p(float v){
    v *= 400;
    int pixels = v;
    pixels += 400;
    return pixels;
}

SpriteSheet* sheet;
SpriteList sprites;
Square* ball;
gfText text;
Data buffer;

void init() {
    running = true;
    buffer = malloc(1024);
    sheet = loadSpriteSheet(ASSETSPATH(atlas.png));
    sprites = spritesFromSheet(*sheet, malloc(4*sizeof(Square)));

    Square* squares = sprites.squareList.squares;
    Position x = {0};
    x.x = p2v(400);
    x.y = p2v(400);
    x.z =0;
    x.rotation =0;
    Square paddle = {0};
    paddle.position = x;
    paddle.width = p2vs(256);
    paddle.height = p2vs(50);
    paddle.u1 = 0.0;
    paddle.u2 = 256.0/512;
    paddle.position.x = p2v(100);
    paddle.position.y = p2v(100);
    paddle.v1= 100.0/256;
    paddle.v2 = 150.0/256;

    squares[0] = paddle;

    paddle.position.x = p2v(700);
    paddle.position.y = p2v(700);
    paddle.v1= 0.0/256;
    paddle.v2 = 50.0/256;
    squares[1] = paddle;

    Square ball_ = {0};
    ball_.position = x;
    ball_.position.x = p2v(500);
    ball_.width = p2vs(100);
    ball_.height = p2vs(100);
    ball_.u1 = 256.0/512;
    ball_.v1 = 0.0;
    ball_.u2 = 384.0/512;
    ball_.v2 = 128.0/256;
    squares[2] = ball_;
    ball = &squares[2];
    sprites.squareList.squares_length = 3;

    glEnable(GL_BLEND);
    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDepthMask(1);


    gf_textStyle style = initTextStyle(ASSETSPATH(Ubuntu-Medium.ttf), 15., 0);
    BBox bbox = {0, 0, 512, 512};
    text.bbox = bbox;
    text.text = malloc(20*sizeof(char));
    strcpy(text.text, "prueba");
    text.style = style;
}

void die() {
    running = false;
}

bool32 hasCollided(Square a, Square b) {
    float leftA = a.position.x - a.width*0.5;
    float rightA = a.position.x + a.width*0.5;
    float topA = a.position.y - a.height*0.5;
    float bottomA = a.position.y + a.height*0.5;

    float leftB = b.position.x - b.width*0.5;
    float rightB = b.position.x + b.width*0.5;
    float topB = b.position.y - b.height*0.5;
    float bottomB = b.position.y + b.height*0.5;

    return ( leftA < rightB && rightA > leftB &&
            topA < bottomB && bottomA > topB);
}

void bounce(float wallDegree, float* vx, float* vy, float incVel) {
    float in = atan2(*vy, *vx);
    float vel = sqrt(*vx* *vx + *vy* *vy);
    float n = wallDegree;
    float out = n + n - in;
    float s = sin(out), c = cos(out);
    float vvx = vel*c;
    float vvy = vel*s;
    *vx = vvx*incVel, *vy = vvy*incVel;
}


Position lastPos;
bool32 startMoving = false;
int points0, points1;
float velX, velY, finalX, finalY, inertia = 1, angle=0;

void resetBall() {
    finalX = 0; finalY = 0; velX = 0; velY =0;
    square_traslateTo(ball, 0, 0);

}
void update() {
    squareList_update_pos(sprites.squareList, buffer);
    float x, y;
    puun_MouseClick mouse;
    puun_KEY key;
    u32 ticks;
    getMousePosition(&x, &y);
    getKeyboardKey(&key);
    getMouseClick(&mouse);
    getTimeElapsed(&ticks);
    float time = ticks / 1000.f; //I Believe this is milliseconds, so I transform into seconds


    //PADDLES
    Square* paddle; int dir =0;
    if(key.isPressed){
        bool32 isKeyAction = true;
        switch(key.key) {
            case 'q': {
                paddle = &sprites.squareList.squares[0];
            }; break;
            case 'w': {
                paddle = &sprites.squareList.squares[1];
            }; break;
            default: isKeyAction = false;
        }
        if(isKeyAction) {
            float x = paddle->position.x + 2*time;
            if(x>1.0) x=-1.0;
            else if(x<-1.0) x=1.0;
            square_traslateTo(paddle, x, paddle->position.y);
        }
    }
    // BALL
    lastPos = ball->position;
    if(mouse & puun_CLICK) {
        if(!startMoving) {
        //    printf("%f %f %f\n", velX, velY, inertia);
            velX =0; velY=0;
            startMoving = true;
        }
        float dx = p2v(x) - lastPos.x ;
        float dy = p2v(y) - (-lastPos.y) ;

        if(!((dx>0) == (velX>0))||((dy>0) == (velY>0))) {
           // printf("Inversion %f %f\n", velX, velY);
            velX*= 0.75, velY *= 0.75;
        }
        velX*= 0.75, velY *= 0.75;
        velX += dx; velY-= dy;
        inertia = time*(1/16.f)*velX*velX+velY*velY;
      //  if(inertia>0.05) finalX=velX, finalY=velY;
     //   else finalX =0, finalY=0;
        if(inertia<10.)inertia=10.;
        finalX=velX, finalY=velY;
        square_traslateTo(ball, p2v(x), -p2v(y));
    }
    else {
        startMoving = false;
        square_traslate(ball, finalX/inertia, finalY/inertia);
    }
    square_rotate(ball,0.01);
    if(ball->position.x<-1) bounce(-TAU/4, &finalX, &finalY, 1);
    if(ball->position.x>+1) bounce(+TAU/4, &finalX, &finalY, 1);
    int i;
    int lastCollided = -1;
    for(i=0; i<2; i++) {
        if(i != lastCollided &&
                hasCollided(*ball, sprites.squareList.squares[i])) {
            lastCollided = i;
            bounce(sprites.squareList.squares[i].position.rotation,
                    &finalX, &finalY, 1);
            break;
        }
    }
    if(ball->position.y < -1) { points0++; resetBall(); }
    if(ball->position.y > +1) { points1++; resetBall(); }
    sprintf(text.text, "%d / %d : %d", points0, points1, ticks);
    lastPos = ball->position;
}

void render() {
    glClearColor(1.0, 1.0, 1.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    renderSpriteList(sprites);
    gfTextRender(text);
    puun_SWAP_BUFFERS();
}
