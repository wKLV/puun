#include <SDL/SDL.h>
#include <SDL/SDL_mixer.h>
#include <GL/glew.h>
#include <stdio.h>
#ifdef JS
#include <emscripten.h>
#endif

void puun_SWAP_BUFFERS() {
    SDL_GL_SwapBuffers();
}

#include "puun.h"
#include "input/keyboard.h"
#include "input/mouse.h"
void sdl_die() {
#ifdef JS
    emscripten_cancel_main_loop();
#endif
    SDL_Quit();
};
#ifdef WDS
#define printf(format, value) \
    {FILE* wdsoutputfile = fopen("log.txt", "a");\
    fprintf(wdsoutputfile, (format), (value));\
    fclose(wdsoutputfile);}
#endif

static float mousePositionX;
static float mousePositionY;

static puun_KEY keyPressed;
static puun_MouseClick isMouseClick;
void sdl_update() {
    SDL_Event event = {0};
    while(SDL_PollEvent(&event)!= 0){
        if(event.type == SDL_QUIT) die();
        else if(event.type == SDL_MOUSEMOTION) {
            //updateMouse(event.motion.x, event.motion.y);
            //TODO: Screen vs Virtual Space
            mousePositionX = event.motion.x;
            mousePositionY = event.motion.y;
        }
        else if(event.type == SDL_MOUSEBUTTONDOWN) {
            if(event.button.button == SDL_BUTTON_LEFT) {
                isMouseClick |= puun_LEFT_CLICK;
            }
            if(event.button.button == SDL_BUTTON_RIGHT) {
                isMouseClick |= puun_RIGHT_CLICK;
            }
            if(event.button.button == SDL_BUTTON_MIDDLE) {
                isMouseClick |= puun_MIDDLE_CLICK;
            }
              //WATCH: SDL2  isMouseClick |= event.button.clicks>1?puun_DOUBLE_CLICK:puun_NOCLICK;
        }
        else if(event.type == SDL_MOUSEBUTTONUP) {
            if(event.button.button == SDL_BUTTON_LEFT) {
                isMouseClick ^= puun_LEFT_CLICK;
            }
            if(event.button.button == SDL_BUTTON_RIGHT) {
                isMouseClick ^= puun_RIGHT_CLICK;
            }
            if(event.button.button == SDL_BUTTON_MIDDLE) {
                isMouseClick ^= puun_MIDDLE_CLICK;
            }
              //WATCH: SDL2  isMouseClick |= event.button.clicks>1?puun_DOUBLE_CLICK:puun_NOCLICK;
        }
        else if(event.type == SDL_KEYUP) {
            keyPressed.isPressed = 0;
        }
        else if(event.type == SDL_KEYDOWN) {
            keyPressed.isPressed = 1;
            keyPressed.key = event.key.keysym.sym;
            keyPressed.alt = puun_NOALT;
            if(event.key.keysym.sym == KMOD_LSHIFT) {
                keyPressed.alt |= puun_SHIFT;
            }
            if(event.key.keysym.sym == KMOD_RSHIFT) {
                keyPressed.alt |= puun_SHIFT;
                keyPressed.alt |= puun_RIGHT_ALTKEY;
            }
            if(event.key.keysym.sym == KMOD_LCTRL) {
                keyPressed.alt |= puun_CONTROL;
            }
            if(event.key.keysym.sym == KMOD_RCTRL) {
                keyPressed.alt |= puun_CONTROL;
                keyPressed.alt |= puun_RIGHT_ALTKEY;
            }
            if(event.key.keysym.sym == KMOD_LALT) {
                keyPressed.alt |= puun_ALT;
            }
            if(event.key.keysym.sym == KMOD_RALT) {
                keyPressed.alt |= puun_ALT;
                keyPressed.alt |= puun_RIGHT_ALTKEY;
            }
            if(event.key.keysym.sym == KMOD_LMETA) {
                keyPressed.alt |= puun_COMMAND;
            }
            if(event.key.keysym.sym == KMOD_RMETA) {
                keyPressed.alt |= puun_COMMAND;
                keyPressed.alt |= puun_RIGHT_ALTKEY;
            }
            if(event.key.keysym.sym == KMOD_NUM) {
                keyPressed.alt |= puun_NUM;
            }
            if(event.key.keysym.sym == KMOD_CAPS) {
                keyPressed.alt |= puun_CAPSLOCK;
            }
            if(event.key.keysym.sym == KMOD_MODE) {
                keyPressed.alt |= puun_FUNCKEY;
            }
        }
    }
    update();
    render();
}
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

#ifdef WDS
int WinMain() {
#else
int main() {
#endif
    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_WM_SetCaption("Puun", NULL);
    if( Mix_OpenAudio( 22050, MIX_DEFAULT_FORMAT, 2, 4096 ) == -1) printf("error initialializ sound\n", 0);
    Mix_Init(MIX_INIT_OGG);
    SDL_SetVideoMode(800, 800, 32, SDL_OPENGL);
    glewInit();
    printf("OpenGL version is (%s)\n", glGetString(GL_VERSION));
    init();
    #ifdef JS
        emscripten_set_main_loop(sdl_update, 0, 0);
    #else
        while(running) sdl_update();
    #endif

    return 0;
}

#include "gf/sprite.c"
#include "gf/text.c"
