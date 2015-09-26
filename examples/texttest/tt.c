#include <stdio.h>
#include <assert.h>
#include <GL/glew.h>
#include <math.h>

//#define STB_IMAGE_WRITE_IMPLEMENTATION
//#include "../other/stb_image_write.h"

#include <other/stb_truetype.h>

#include <puun/puun.h>
#include <puun/gf/gl_help.h>
#include <puun/gf/square.h>
#include <puun/gf/text.h>


#ifndef ASSETSPATH
#define ASSETSPATH(name) "../assets/" # name
#endif

//char buffer[24<<20];

void init(Data game_memory){
//    u8* Image = malloc(1<<18);
//    s32 ImageWidth; s32 ImageHeight;
    // not_main();
#if 0
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
#endif
    gf_textStyle style = initTextStyle((u8*)ASSETSPATH(Ubuntu-Medium.ttf), 24., 0);
    gfText text;
    Rectangle bbox = ZERO_STRUCT;
    bbox.x1 = -256;
    bbox.y1 = 0;
    bbox.x2 = -56;
    bbox.y2 = 250;
    //text = {0}; //FIX:WDS COMPLAIN: {bbox, "HELLO WORLD      jump line", style};
    text.bbox = bbox;
    text.text = (u8*)"HELLO WORLD         From the oTHER Side                  ";
    text.style = style;

    printf("fit: %d\n", gfTextFit(text));

    glClearColor(1.0, 1.0, 1.0, 1.0);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    gfTextRender(text);
    *(gfText*)game_memory = text;
#if 0
    stbtt_fontinfo font;
    u8* buffer = malloc(1000000);
    assert(fread(buffer, 1, 1000000, fopen(ASSETSPATH(Ubuntu-Light.ttf), "rb")));
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
    Texts.squares[0].position.x = 0; Texts.squares[0].position.y = 0; Texts.squares[0].position.z = 0;
    Texts.squares[0].width = 1;
    Texts.squares[0].height = 1;
    Texts.squares[0].u1 = 0; Texts.squares[0].u2 = 1;
    Texts.squares[0].v1 = 0; Texts.squares[0].v2 = 1;

    squareList_update_pos(Texts, buffer);
#endif
}

void updateNrender(Data game_memory){
    gfText text = *(gfText*)game_memory;
   // glClearColor(1.0, 1.0, 1.0, 1.0);
   // glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

 
//    UniformData uniImg = {0};
//    uniImg.name = "texture";
//    uniImg.dataStructure = Texture;
//    uniImg.texnum = 0;
//   uniImg.id = ImageId;


    //render_squareList(Texts, (Data)&uniImg, 1);
    //uniImg.id = ballId;
    //uniImg.texnum = 0;
    //render_squareList(Squares, (Data)&uniImg, 1);
    u32 ms;
    static float totalTime;
    getTimeElapsed(&ms);
    float time = ms/1000.f;
    totalTime += time;
    char contentText[256];
    //f32 x1 = 256*sinf(0.5f*totalTime)-128;
    f32 y1 = 70*cosf(0.5f*totalTime*0.3);
    f32 w = 512*sinf(0.75f*totalTime+TAU/2.0f)+512+40;
    f32 h = 512*cosf(0.75f*totalTime+TAU/2.0f)+512+120;
    text.bbox = rectangle_topleft_by_widtheight(-256.f, y1, w, h);
    b32 fits = gfTextFit(text);
    assert(fits);
    sprintf(contentText, "HELLO WORLD %d  %u     From the oTHER Side", fits, ms);
    text.text= (u8*)contentText;
    glClearColor(0.5, 0.75, 0.25, 1.0);
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    gfTextRender(text);
    puun_SWAP_BUFFERS();
}

void game_die(Data game_memory){
    platform_die();
}
