#ifndef PUUN_H
#define PUUN_H
#include "input/keyboard.h"
#include "input/mouse.h"
#include "sound/sound.h"
char running;

void init();

void update();

void getMousePosition(float* x, float* y);
void getKeyboardKey(puun_KEY* character);
void getMouseClick(puun_MouseClick* click);

void render();

void puun_SWAP_BUFFERS();

void die();
#endif
