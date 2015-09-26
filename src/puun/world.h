#include <puun/types.h>
#include <puun/math_types.h>

struct Camera {
    Rectangle projected;
};

typedef struct Camera Camera;

struct Game_World {
    u32 sizeX;
    u32 sizeY;


    Camera mainCamera;
};
typedef struct Game_World Game_World;
