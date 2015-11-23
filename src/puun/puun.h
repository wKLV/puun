#ifndef PUUN_H
#define PUUN_H
#include "input/keyboard.h"
#include "input/mouse.h"
#include "sound/sound.h"

extern void init();

extern void updateNrender();

extern void game_die();
extern void platform_die();

void getMousePosition(float* x, float* y);
void getKeyboardKey(puun_KEY* character);
void getMouseClick(puun_MouseClick* click);
void getTimeElapsed(unsigned int* time);
unsigned int getMiliSecondsFromFirst();

void puun_SWAP_BUFFERS();

#endif
