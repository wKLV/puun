#include "../types.h"
#include "square.h"

typedef struct {
    s32 id;
    u8 margin_left;
    u8 margin_top;
    u8 padding_x;
    u8 padding_y;
} SpriteSheet;

SpriteSheet* loadSpriteSheet(String loc);

typedef struct {
    SquareList squareList;
    u32 ImageId;
} SpriteList;

SpriteList spritesFromSheet(SpriteSheet s, Data squares);

void renderSpriteList(SpriteList sl);
