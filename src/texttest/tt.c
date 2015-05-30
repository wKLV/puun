#include <stdio.h>
#include <assert.h>

//#include "../other/stb_truetype.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "../other/stb_image_write.h"

#include "sdl_puun.h"
#include "../puun/gf/square2.c"
#include "../puun/gf/text.c"

//char buffer[24<<20];

static SquareList Texts;
static gfText text;
GLuint ImageId;
void init(){
    u8* Image = malloc(1<<18);
    s32 ImageWidth; s32 ImageHeight;
    running = true;
    // not_main();
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
                                 float c = texture2D(texture, texcoord).a;\n\
                                 c = 1. -c;\n\
                                 gl_FragColor = vec4(c, c, c, 0);\n\
                             }\n\
    ";

    gf_textStyle style = initTextStyle("assets/Ubuntu-Light.ttf", 35., 0);
    BBox bbox = {0, 0, 512, 512};
    //text = {0}; //FIX:WDS COMPLAIN: {bbox, "HELLO WORLD      jump line", style};
    text.bbox = bbox;
    text.text = "HELLO WORLD      jump line";
    text.style = style;


    printf("fit: %d\n", gfTextFit(text));

    glClearColor(1.0, 1.0, 1.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    gfTextRender(text);

    stbtt_fontinfo font;
    char* buffer = malloc(1000000);
    fread(buffer, 1, 1000000, fopen("assets/Ubuntu-Light.ttf", "rb"));
    stbtt_InitFont(&font, buffer, 0);
    ImageWidth = 512, ImageHeight = 512;
    float scale = stbtt_ScaleForPixelHeight(&font, 35);
    stbtt_MakeCodepointBitmap(&font, Image, ImageWidth, ImageHeight, ImageHeight, scale, scale, 'j');
    //1, 1, 85, &ImageWidth, &ImageHeight, NULL, NULL);
    stbi_write_png("prueba", ImageWidth, ImageHeight, 1, Image, ImageWidth);
    printf("%f %d %d\n", scale, ImageWidth, ImageHeight);
    u8 program = setupProgram(vertexSource, sizeof(vertexSource), fragmentSource, sizeof(fragmentSource));

  //  ImageId = setupTexture_Any(Image, ImageWidth, ImageHeight, GL_LUMINANCE, GL_RED);
    glGenTextures(1, &ImageId);

    glBindTexture(GL_TEXTURE_2D, ImageId);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA, ImageWidth, ImageHeight, 0, GL_ALPHA, GL_UNSIGNED_BYTE, Image);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glGenerateMipmap(GL_TEXTURE_2D);
    Texts = create_square_list(program, calloc(1,sizeof(Square)));
    Texts.program = program;
    Texts.squares_length = 1;
    Texts.squares[0].x = 0; Texts.squares[0].y = 0; Texts.squares[0].z = 0;
    Texts.squares[0].width = 1;
    Texts.squares[0].height = 1;
    Texts.squares[0].u1 = 0; Texts.squares[0].u2 = 1;
    Texts.squares[0].v1 = 0; Texts.squares[0].v2 = 1;

    squareList_update_pos(Texts, buffer);
}

void update(){

}

void updateMouse(int x, int y){

}

void render(){
    glClearColor(1.0, 1.0, 1.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

    gfTextRender(text);
//    UniformData uniImg = {0};
//    uniImg.name = "texture";
//    uniImg.dataStructure = Texture;
//    uniImg.texnum = 0;
//   uniImg.id = ImageId;


    //render_squareList(Texts, (Data)&uniImg, 1);
    //uniImg.id = ballId;
    //uniImg.texnum = 0;
    //render_squareList(Squares, (Data)&uniImg, 1);

    SDL_GL_SwapBuffers();
}

void die(){
    running = false;
}
