#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include <SDL/SDL.h>
#include <GL/glew.h>
#ifdef JS
#include <emscripten.h>
#endif

#include "sdl_puun.h"
#include "../puun/gf/square2.c"
#define STB_IMAGE_IMPLEMENTATION
#include "../other/stb_image.h"

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
SquareList Squares;
u8 program;

static GLuint ImageId;
static u8* Image;

void render() {
    glClearColor(1.0, 1.0, 1.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

    UniformData uniImg = {0};
    uniImg.name = "texture";
    uniImg.dataStructure = Texture;
    uniImg.texnum = 0;
    uniImg.id = ImageId;


    render_squareList(Squares, (Data)&uniImg, 1);
    //uniImg.id = ballId;
    //uniImg.texnum = 0;
    //render_squareList(Squares, (Data)&uniImg, 1);

    SDL_GL_SwapBuffers();
}

static Data buffer;

void init() {
    running = true;
    Data d = malloc(4*sizeof(Square));
    buffer = malloc(4000);
        char vertexSource[] = "attribute vec3 position;\n\
                               attribute vec2 uv;\n\
                               attribute vec2 scale;\n\
                               varying vec2 texcoord;\n\
                               void main() {\n\
                                   gl_Position = vec4(position, 1.0);\n\
                                   texcoord = uv;\n\
                               }";
        char fragmentSource[] = "precision mediump float; \n\
                                 uniform sampler2D texture; \n\
                                varying vec2 texcoord; \n\
                               void main() {\n\
                               gl_FragColor = texture2D(texture, texcoord);\n\
                               }\n\
                              ";
    program = setupProgram(vertexSource, sizeof(vertexSource), fragmentSource, sizeof(fragmentSource));
    Squares = create_square_list(program, malloc(3*sizeof(Square)));
    int x, y, n;
    Image = stbi_load("assets/atlas.png", &x, &y, &n, 0);
    ImageId = setupTexture(Image, x, y);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDepthMask(1);

    Square s = {0};
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
static float  paddleX =0, paddleY=-.8, paddleRot;

void updateMouse(int x, int y) {
    paddleX = s2p(x);
    paddleRot = s2p(y);
}


void update(){
    static float rotate = 0;
    static float vx = 0, vy = -1, px = 0, py = 0, av =0;
    static char hasBounced = 0;

    rotate += av*.01;
    px += 0.01*vx, py += 0.01*vy;
    square_rotate(ball, rotate);
    square_traslate(ball, px, py);

    square_traslate(paddle, paddleX, paddleY);
    square_rotate(paddle, paddleRot);

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
}

void die() {
    //free(buffer);
    running = false;
    printf("YOUR SCORE IS %d\n", score);
 //   sdl_die();
}
