#ifndef PUUN_SDLH
#define PUUN_SDLH

typedef enum puun_MouseClick {
    puun_NOCLICK = 0,
    puun_LEFT_CLICK = 1,
    puun_RIGHT_CLICK = 1 << 1,
    punn_MIDDLE_CLICK = 1 << 2,
    puun_DOUBLE_CLICK = 1 << 3,
    puun_A_ACTION = 1 << 4,
    puun_B_ACTION = 1<< 5,
    puun_X_ACTION = 1<<6,
    puun_Y_ACTION = 1<<7,
    puun_LEFT_SHOULDER = 1<<8,
    puun_RIGHT_SHOULDER = 1<<9,
    puun_LEFT_STICK_CLICK = 1<<10,
    puun_RIGHT_STICK_CLICK = 1<<11,
    puun_LEFT_ACTION = 1<<12,
    puun_RIGHT_ACTION = 1<<13,
    puun_UP_ACTION = 1<<14,
    puun_DOWN_ACTION = 1<<15
} puun_MouseClick;

typedef enum puun_ALTKEY {
    puun_NOALT = 0,
    puun_CONTROL = 1,
    puun_ALT = 1<<1,
    puun_SHIFT = 1<<2,
    puun_COMMAND = 1<<3,
    puun_RIGHT_ALTKEY = 1<<4,
    puun_CAPSLOCK = 1<<5,
    puun_FUNCKEY = 1<<6,
    puun_NUM = 1<<7
} puun_ALTKEY;

typedef struct puun_KEY {
    char isPressed;
    char key;
    puun_ALTKEY alt;
} puun_KEY;
char running;

void sdl_die();

void init();

void update();

void updateMouse(int x, int y);

void getKeyboardKey(puun_KEY* key);
void getMousePosition(float* x, float* y);
void getMouseClick(puun_MouseClick* click);

void render();

void die();
#endif
