#ifndef puunSquare
#define puunSquare

#include "../types.h"

typedef struct {
    float x;
    float y;
    float z;
    float rotation;
} Position;

typedef struct {
    Position position;
    float width;
    float height;

    float u1;
    float v1;
    float u2;
    float v2;
} Square;

Square create_square(float width, float height);

void square_traslate(Square* square, float x, float y);
void square_rotate(Square* square, float angle);
void square_traslateTo(Square* square, float x, float y);
void square_rotateTo(Square* square, float angle);

typedef struct {
   // Square* squares;
   // s32 squares_length;
    DEF_ARR(squares, Square)

    u8 program;
    u32 pos_buffer;
} SquareList;


SquareList create_square_list(u8 program, Data squares);

void squareList_update_pos(SquareList sl, Data d);

void render_squareList(SquareList sl, Data* unis,
        s32 unis_length);
#endif
