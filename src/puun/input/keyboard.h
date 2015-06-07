#ifndef puun_keyboard
#define puun_keyboard
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
#endif
