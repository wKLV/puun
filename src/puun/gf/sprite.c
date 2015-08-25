#include "sprite.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include "gl_help.h"
#define STB_IMAGE_IMPLEMENTATION
#include "../../other/stb_image.h"
#include "square2.c"

struct SpriteSheetData {
    GLuint ImageId;
    SpriteSheet spritesheet;
};


struct SpriteSheetData spritesheets[512] = {};
s32 spritesheets_length = 0;
GLuint Program;
GLuint Buffer;

SpriteSheet* loadSpriteSheet(String loc) {
    GLuint ImageId;
    u8* Image;
    int x, y, n;
    Image = stbi_load(loc, &x, &y, &n, 0);
    ImageId = setupTexture(Image, x, y);

    struct SpriteSheetData data = {};
    data.ImageId = ImageId;
    assert(spritesheets_length<512);
    s32 id = spritesheets_length++;
    SpriteSheet sheet = {};
    sheet.id = id;
    data.spritesheet = sheet;

    spritesheets[id] = data;
    return &spritesheets[id].spritesheet;
}

SpriteList spritesFromSheet(SpriteSheet s, Data squares){
    u8 vs[] = "attribute vec2 position; attribute vec2 uv;\n\
                 varying vec2 texcoord; void main(){\n\
                     gl_Position=vec4(position, 0, 1);\n\
                    texcoord=uv;}";
    u8 fs[] = "precision mediump float; \n\
                 uniform sampler2D texture; \n\
                 varying vec2 texcoord; \n\
                 void main() {\n\
                     gl_FragColor = texture2D(texture, texcoord);\n\
                 }";

    Program = setupProgram(vs, 0, fs, 0);
    SquareList sl = create_square_list(Program, squares);

    SpriteList splist = {};
    splist.squareList = sl;
    splist.ImageId = spritesheets[s.id].ImageId;
    return splist;
}

void renderSpriteList(SpriteList sl) {
    glUseProgram(sl.squareList.program);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, sl.ImageId);
    glUniform1i(
            glGetUniformLocation(sl.squareList.program,
                "texture"),
            0);

    glDrawArrays(GL_TRIANGLES, 0, 6*sl.squareList.squares_length);
}
//TODO Still need this?
#if 0
u32 spritePosition(u8* name) {
    u64 hash = 0xcbf29ce484222325;
    int i;
    for(i=0; name[i]; i++) {
        hash += 0x100000001b3;
        hash ^= name[i];
    }
    return (hash % 1024);
}

#endif
