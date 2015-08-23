#ifndef puuntexth
#define puuntexth
#include "../types.h"

typedef struct {
    u8 id;
    f32 pixels;
    u32 color; // RGBA packed, each u8
    //TODO Add options``
} gf_textStyle;

struct gfCharacterData {
    f32 beggining;
    f32 end;
    int width;
    int height;
    int offsetx;
    int offsety;
};
#define gf_TEXT_STRIDE (64)
#define gf_TEXT_LENGTH (2048)

typedef u8 VerticalMatrixu8 [gf_TEXT_LENGTH][gf_TEXT_STRIDE];
struct gfFontData {
    Data font;
    VerticalMatrixu8* atlas;
    Data cdata;
    u32 textureId;
    u32 glProgram;
    u32 glBuffer;
    struct gfCharacterData characters[255];
};

typedef struct {
    BBox bbox;
    u8* text;
    gf_textStyle style;
} gfText;

gf_textStyle initTextStyle(u8* font_pos,
        f32 pixels, u32 color);

BBox gfTextNeeded(u8* text, gf_textStyle style);

bool gfTextFit(gfText text);

void gfTextRenderChungo(gfText text);
void gfTextRender(gfText text);
#endif
