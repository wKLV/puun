#include "GL/glew.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>

#include <puun/puun.h>
#include <puun/types.h>
#include <puun/gf/sprite.h>
#include <puun/gf/text.h>

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

struct Game_Memory {
    SpriteSheet* sheet;
    SpriteList sprites;
    Square* ball;
    gfText text;
    u8 buffer[1024];
    Position lastPos;
    b32 startMoving;
    int points0, points1;
    float velX, velY, finalX, finalY, inertia, angle;
    Square square_data[4];
};

void init(Data game_memory) {
    struct Game_Memory* mem = (struct Game_Memory*) game_memory;

    mem->sheet = loadSpriteSheet(ASSETSPATH(atlas.png));
    mem->sprites = spritesFromSheet(*mem->sheet, mem->square_data);

    Square* squares = mem->sprites.squareList.squares;
    Position x = ZERO_STRUCT;
    x.x = p2v(400);
    x.y = p2v(400);
    x.z =0;
    x.rotation =0;
    Square paddle = ZERO_STRUCT;
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

    Square ball_ = ZERO_STRUCT;
    ball_.position = x;
    ball_.position.x = p2v(500);
    ball_.width = p2vs(100);
    ball_.height = p2vs(100);
    ball_.u1 = 256.0/512;
    ball_.v1 = 0.0;
    ball_.u2 = 384.0/512;
    ball_.v2 = 128.0/256;
    squares[2] = ball_;
    mem->ball = &squares[2];
    mem->sprites.squareList.squares_length = 3;

    glEnable(GL_BLEND);
    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDepthMask(1);


    gf_textStyle style = initTextStyle((u8*)ASSETSPATH(Ubuntu-Medium.ttf), 15., 0);
    Rectangle bbox = rectangle_topleft_to_bottomright(0, 0, 512, 512);
    mem->text.bbox = bbox;
    mem->text.text = malloc(20*sizeof(char));
    strcpy((char*)mem->text.text, "prueba");
    mem->text.style = style;

    mem->inertia = 1.0f;
}

void game_die() {
    platform_die();
}

b32 hasCollided(Square a, Square b) {
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



void resetBall(struct Game_Memory* mem) {
    mem->finalX = 0; mem->finalY = 0; mem->velX = 0; mem->velY =0;
    square_traslateTo(mem->ball, 0, 0);

}
void updateNrender(Data game_memory) {
    struct Game_Memory* mem = (struct Game_Memory*) game_memory;
    squareList_update_pos(mem->sprites.squareList, mem->buffer);
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
    Square* paddle;
    if(key.isPressed){
        b32 isKeyAction = true;
        switch(key.key) {
            case 'q': {
                paddle = &mem->sprites.squareList.squares[0];
            }; break;
            case 'w': {
                paddle = &mem->sprites.squareList.squares[1];
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
    mem->lastPos = mem->ball->position;
    if(mouse & puun_CLICK) {
        if(!mem->startMoving) {
        //    printf("%f %f %f\n", velX, velY, inertia);
            mem->velX =0; mem->velY=0;
            mem->startMoving = true;
        }
        float dx = p2v(x) - mem->lastPos.x ;
        float dy = p2v(y) - (-mem->lastPos.y) ;

        if(!((dx>0) == (mem->velX>0))||((dy>0) == (mem->velY>0))) {
           // printf("Inversion %f %f\n", velX, velY);
            mem->velX*= 0.75, mem->velY *= 0.75;
        }
        mem->velX*= 0.75, mem->velY *= 0.75;
        mem->velX += dx; mem->velY-= dy;
        mem->inertia = time*(1/16.f)*mem->velX*mem->velX+mem->velY*mem->velY;
      //  if(inertia>0.05) finalX=velX, finalY=velY;
     //   else finalX =0, finalY=0;
        if(mem->inertia<10.)mem->inertia=10.;
        mem->finalX=mem->velX, mem->finalY=mem->velY;
        square_traslateTo(mem->ball, p2v(x), -p2v(y));
    }
    else {
        mem->startMoving = false;
        square_traslate(mem->ball, mem->finalX/mem->inertia, mem->finalY/mem->inertia);
    }
    square_rotate(mem->ball,0.01);
    if(mem->ball->position.x<-1) bounce(-TAU/4, &mem->finalX, &mem->finalY, 1);
    if(mem->ball->position.x>+1) bounce(+TAU/4, &mem->finalX, &mem->finalY, 1);
    int i;
    int lastCollided = -1;
    for(i=0; i<2; i++) {
        if(i != lastCollided &&
                hasCollided(*mem->ball, mem->sprites.squareList.squares[i])) {
            lastCollided = i;
            bounce(mem->sprites.squareList.squares[i].position.rotation,
                    &mem->finalX, &mem->finalY, 1);
            break;
        }
    }
    if(mem->ball->position.y < -1) { mem->points0++; resetBall(mem); }
    if(mem->ball->position.y > +1) { mem->points1++; resetBall(mem); }
    sprintf((char*)mem->text.text, "%d / %d : %ums", mem->points0, mem->points1, ticks);
    mem->lastPos = mem->ball->position;

    glClearColor(1.0, 1.0, 1.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    renderSpriteList(mem->sprites);
    gfTextRender(mem->text);
    puun_SWAP_BUFFERS();
}
