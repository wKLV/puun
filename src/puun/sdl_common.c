void puun_SWAP_BUFFERS() {
    SDL_GL_SwapBuffers();
}
static b32 running;
static Data game_memory;
#include "puun.h"
#include "input/keyboard.h"
#include "input/mouse.h"
void platform_die() {
    running = false;
#ifdef JS
    emscripten_cancel_main_loop();
#endif
    SDL_Quit();
};
#if 0 //This was a horrible idea //#TODO:40 Logger
#ifdef WDS
#define printf(format, value) \
    {FILE* wdsoutputfile = fopen("log.txt", "a");\
    fprintf(wdsoutputfile, (format), (value));\
    fclose(wdsoutputfile);}
#endif
#endif

static float mousePositionX;
static float mousePositionY;

static puun_KEY keyPressed;
static puun_MouseClick isMouseClick;
void getMousePosition(float* x, float* y) {
    *x = mousePositionX;
    *y= mousePositionY;
}
void getKeyboardKey(puun_KEY* character) {
    *character = keyPressed;
}
void getMouseClick(puun_MouseClick* click) {
    *click = isMouseClick;
}

u32 ticks;
void getTimeElapsed(u32* time) {
    u32 nextTicks = SDL_GetTicks();
    *time = nextTicks - ticks;
    ticks = nextTicks;
}

Mix_Music* music[512];
int music_length;
Mix_Chunk* chunks[1024];
int chunks_length;

puun_SoundId loadSoundFile(char* path) {
    chunks[chunks_length] = Mix_LoadWAV(path);
    return chunks_length++;
}
void playSound(puun_SoundId id) {
    Mix_PlayChannel( -1, chunks[id], 0);
}

