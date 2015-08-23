#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include <SDL/SDL.h>
#include <GL/glew.h>

#include "../puun/puun.h"
#include "../puun/gf/square.h"
#include "../puun/gf/gl_help.h"
#include "../other/stb_image.h"

#ifndef ASSETSPATH
#define ASSETSPATH(name) "../assets/" # name
#endif

#define TAU 6.2832

Square* paddle;
Square* ball;
SquareList Squares;
u8 program;

static GLuint ImageId;
static u8* Image;

static Data buffer;

void init() {
    running = true;
    buffer = malloc(4000);
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
    program = setupProgram(vertexSource, sizeof(vertexSource), fragmentSource, sizeof(fragmentSource));
    Squares = create_square_list(program, malloc(3*sizeof(Square)));
    int x, y, n;
    Image = stbi_load(ASSETSPATH(atlas.png), &x, &y, &n, 0);
    ImageId = setupTexture(Image, x, y);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDepthMask(1);

    Square s = {};
    s.width = 0.8; s.height = 0.1;
    s.u1 = 0./512; s.u2= 256./512;
    s.v1 = 0./256; s.v2 = 49./256;
    s.position.z = +0.8;
    Squares.squares[0] = s;
    s.width = 0.2; s.height = 0.2;
    s.u1 = 256./512; s.u2= 384./512;
    s.v1 = 0./256; s.v2 = 128./256;
    s.position.z = 0;
    Squares.squares[1] = s;
    Squares.squares_length = 2;

    paddle = &Squares.squares[0];
    ball = &Squares.squares[1];

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
static float paddleX =0, paddleY=-.8, paddleRot;

static float rotate = 0;
static float vx = 0, vy = -1, px = 0, py = 0, av =0;
static char hasBounced = 0;
void updateNrender(){
    float time;
    {
        float x, y;
        u32 ms;
        getMousePosition(&x, &y);
        paddleX = s2p(x);
        paddleRot = s2p(y);
        getTimeElapsed(&ms);
        time = ms /1000.f;
    }

    rotate += av*time*1;
    px += 1*time*vx, py += 1*time*vy;
    square_rotateTo(ball, rotate);
    square_traslateTo(ball, px, py);

    square_traslateTo(paddle, paddleX, paddleY);
    square_rotateTo(paddle, paddleRot);

    squareList_update_pos(Squares, buffer);

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



    glClearColor(1.0, 1.0, 1.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

    UniformData uniImg = {};
    uniImg.name = (u8*)"texture";
    uniImg.dataStructure = Texture;
    uniImg.texnum = 0;
    uniImg.id = ImageId;


    render_squareList(Squares, (Data)&uniImg, 1);
    //uniImg.id = ballId;
    //uniImg.texnum = 0;
    //render_squareList(Squares, (Data)&uniImg, 1);

    puun_SWAP_BUFFERS();
}

void die() {
    //free(buffer);
    running = false;
    printf("YOUR SCORE IS %d\n", score);
 //   sdl_die();
}
