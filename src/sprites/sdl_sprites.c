#include <SDL/SDL.h>
#include <GL/glew.h>
#include <stdio.h>
#ifdef JS
#include <emscripten.h>
#endif

#include "sdl_puun.h"
void sdl_die() {
#ifdef JS
    emscripten_cancel_main_loop();
#endif
    SDL_Quit();
};

void sdl_update() {
    SDL_Event event = {};
    while(SDL_PollEvent(&event)!= 0){
        if(event.type == SDL_QUIT) running = 0;//die();
        if(event.type == SDL_MOUSEMOTION) {
            updateMouse(event.motion.x, event.motion.y);
        }
    }
    update();
    render();
}


int main() {
    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_WM_SetCaption("Secondy puun", NULL);
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
#include "sprite.c"
