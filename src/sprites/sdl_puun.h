#ifndef PUUN_SDLH
#define PUUN_SDLH
char running;

void sdl_die();

void init();

void update();

void updateMouse(int x, int y);

void getKeyboardKey(char* character);
void getMousePosition(float* x, float* y);
void getMouseClick(char click);

void render();

void die();
#endif
