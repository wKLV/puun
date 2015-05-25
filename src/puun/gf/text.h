#include "../types.h"

typedef struct {
    u8 id;
    f32 pixels;
    u32 color; // RGBA packed, each u8
    //TODO Add options``
} gf_textStyle;

struct gfFontData {
    Data font;
    Data atlas;
    Data cdata;
    u32 textureId;
    u32 glProgram;
    u32 glBuffer;
};

struct gfFontData gfTextStyles[255] = {};
u8 gfTextStyles_length = 0;

typedef struct {
    BBox bbox;
    char* text;
    gf_textStyle style;
} gfText;

gf_textStyle initTextStyle(char* font_pos,
        f32 pixels, u32 color);

BBox gfTextNeeded(char* text, gf_textStyle style);

bool gfTextFit(gfText text);

void gfTextRender(gfText text);
