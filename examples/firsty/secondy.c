#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include <SDL/SDL.h>
#include <GL/glew.h>

#include <puun/puun.h>
#include <puun/gf/square.h>
#include <puun/gf/gl_help.h>
#include <other/stb_image.h>

#ifndef ASSETSPATH
#define ASSETSPATH(name) "../assets/" # name
#endif

struct Game_Memory {
    u32 ImageId;

    int score;
    float paddleX;
    float paddleY;
    float paddleRot;
    float rotate;
    float vx, vy, px, py, av;
    b32 hasBounced;

    Square* paddle;
    Square* ball;
    SquareList Squares;
    u8 program;
    u8 buffer[4000];
};


void init(Data game_memory) {
    struct Game_Memory* mem = (struct Game_Memory*) game_memory;

    mem->paddleX=0;
    mem->paddleY=-0.8;
    mem->paddleRot=0;
    mem->paddleX=0;
    mem->vy=1;
    u32 ms;
    getTimeElapsed(&ms);
    u8 vertexSource[] = "attribute vec3 position;\n\
                           attribute vec2 uv;\n\
                           attribute vec2 scale;\n\
                           varying vec2 texcoord;\n\
                           void main() {\n\
                               gl_Position = vec4(position, 1.0);\n\
                               texcoord = uv;\n\
                           }";
    u8 fragmentSource[] = "precision mediump float; \n\
                             uniform sampler2D texture; \n\
                            varying vec2 texcoord; \n\
                           void main() {\n\
                           gl_FragColor = texture2D(texture, texcoord);\n\
                           }\n\
                          ";
    mem->program = setupProgram(vertexSource, sizeof(vertexSource), fragmentSource, sizeof(fragmentSource));
    mem->Squares = create_square_list(mem->program, malloc(3*sizeof(Square)), 2);
    int x, y, n;
    u8* Image = stbi_load(ASSETSPATH(atlas.png), &x, &y, &n, 0);
    mem->ImageId = setupTexture(Image, x, y);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDepthMask(1);

    Square s = ZERO_STRUCT;
    s.width = 0.8; s.height = 0.1;
    s.u1 = 0./512; s.u2= 256./512;
    s.v1 = 0./256; s.v2 = 49./256;
    s.position.z = +0.8;
    mem->Squares.squares[0] = s;
    s.width = 0.2; s.height = 0.2;
    s.u1 = 256./512; s.u2= 384./512;
    s.v1 = 0./256; s.v2 = 128./256;
    s.position.z = 0;
    mem->Squares.squares[1] = s;

    mem->paddle = &mem->Squares.squares[0];
    mem->ball = &mem->Squares.squares[1];

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

void updateNrender(Data game_memory){
    struct Game_Memory* mem = (struct Game_Memory*) game_memory;
    float time;
    {
        float x, y;
        u32 ms;
        getMousePosition(&x, &y);
        mem->paddleX = s2p(x);
        mem->paddleRot = s2p(y);
        getTimeElapsed(&ms);
        time = ms /1000.f;
    }

    mem->rotate += mem->av*time*1;
    mem->px += 1*time*mem->vx, mem->py += 1*time*mem->vy;
    square_rotateTo(mem->ball, mem->rotate);
    square_traslateTo(mem->ball, mem->px, mem->py);

    square_traslateTo(mem->paddle, mem->paddleX, mem->paddleY);
    square_rotateTo(mem->paddle, mem->paddleRot);

    squareList_update_pos(mem->Squares, mem->buffer);

    //CHECK COLLISION :)
    if(mem->hasBounced!= 1 && mem->px <-1){ bounce(TAU/4, &mem->vx, &mem->vy, &mem->av, 1.);mem->hasBounced = 1; }
    if(mem->hasBounced!=2 && mem->px> 1){ bounce(-TAU/4, &mem->vx, &mem->vy, &mem->av, 1.);mem->hasBounced = 2;}
    if(mem->hasBounced!=3 && mem->py >1){ bounce(-TAU/2, &mem->vx, &mem->vy, &mem->av, 1.); mem->hasBounced = 3; }
    if(mem->py <-1){
        game_die(game_memory); return;
    }
    if(mem->hasBounced!=4 && mem->px<= mem->paddleX+0.4 && mem->px>=mem->paddleX-0.4  && mem->py<=mem->paddleY+0.05 && mem->py>=mem->paddleY-0.05) {
        bounce(3/4*TAU-mem->paddleRot, &mem->vx, &mem->vy, &mem->av, 1.2);
        mem->hasBounced = 4;
        mem->score++;
    }



    glClearColor(1.0, 1.0, 1.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

    UniformData uniImg = ZERO_STRUCT;
    uniImg.name = (u8*)"texture";
    uniImg.dataStructure = Texture;
    uniImg.texnum = 0;
    uniImg.id = mem->ImageId;


    render_squareList(mem->Squares, (Data)&uniImg, 1);
    //uniImg.id = ballId;
    //uniImg.texnum = 0;
    //render_squareList(Squares, (Data)&uniImg, 1);

    puun_SWAP_BUFFERS();
}

void game_die(Data game_memory) {
    struct Game_Memory* mem = (struct Game_Memory*) game_memory;
    //free(buffer);
    printf("YOUR SCORE IS %d\n", mem->score);
    platform_die();
}
