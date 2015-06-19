#ifndef puun_mouse
#define puun_mouse
typedef enum puun_MouseClick {
    puun_NOCLICK = 0,
    puun_LEFT_CLICK = 1,
    puun_RIGHT_CLICK = 1 << 1,
    puun_MIDDLE_CLICK = 1 << 2,
    puun_CLICK = puun_LEFT_CLICK | puun_RIGHT_CLICK | puun_MIDDLE_CLICK,
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
#endif
