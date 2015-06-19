#include <SDL/SDL.h>
#include <SDL/SDL_mixer.h>
#include <GL/glew.h>
#include <stdio.h>
#ifdef JS
#include <emscripten.h>
#endif

#ifdef WDS
#define printf(format, value) \
    {FILE* wdsoutputfile = fopen("log.txt", "a");\
    fprintf(wdsoutputfile, (format), (value));\
    fclose(wdsoutputfile);}
#endif

#include "sdl_puun.h"
Mix_Music* music = NULL;

void sdl_die() {
#ifdef JS
    emscripten_cancel_main_loop();
#endif
    Mix_FreeMusic(music);
    Mix_CloseAudio();
    SDL_Quit();
};

void sdl_update() {
    static SDL_Event event;
    while(SDL_PollEvent(&event)!= 0){
        if(event.type == SDL_QUIT) die();
        if(event.type == SDL_MOUSEMOTION) {
            updateMouse(event.motion.x, event.motion.y);
        }
    }
    update();
    render();
}


void loadnPlaySound(char* filename) {
    music = Mix_LoadMUS(filename);
    if(music == NULL) printf("error loading %s\n", filename);
    if( Mix_PlayMusic(music, -1) == -1) printf("error playing %s\n", filename);
}

#ifdef WDS
int WinMain() {
#else
int main() {
#endif
    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_WM_SetCaption("Firsty puun", NULL);
    SDL_SetVideoMode(800, 800, 32, SDL_OPENGL);
    if( Mix_OpenAudio( 22050, MIX_DEFAULT_FORMAT, 2, 4096 ) == -1) printf("error initialializ sound\n", 0);
//    Mix_Init(MIX_INIT_MOD|MIX_INIT_MP3|MIX_INIT_OGG);
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
#include "firsty.c"
