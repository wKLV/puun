#include <SDL/SDL.h>
#include <SDL/SDL_mixer.h>
#include <GL/glew.h>
#include <stdio.h>
#include "types.h"
#include <windows.h>

typedef void GameFunction(Data game_memory);
static GameFunction* myUpdateNrender;
static GameFunction* myInit;
static GameFunction* myGame_die;
static HMODULE GameCodeDLL;
static FILETIME DLLLastWriteTime;

FILETIME get_last_write_time(char* file)
{
    FILETIME lastWriteTime = ZERO_STRUCT;

    WIN32_FILE_ATTRIBUTE_DATA data;
    if(GetFileAttributesEx(file, GetFileExInfoStandard, &data))
    {
        lastWriteTime = data.ftLastWriteTime;
    }

    return lastWriteTime;
}

static b32 puun_load_game(char* path) {
    b32 result = false;
    GameCodeDLL = LoadLibrary(path);
    if(GameCodeDLL)
    {
        myUpdateNrender = (GameFunction*)
            GetProcAddress(GameCodeDLL, "updateNrender");

        myInit = (GameFunction*)
            GetProcAddress(GameCodeDLL, "init");

        myGame_die = (GameFunction*)
            GetProcAddress(GameCodeDLL, "game_die");

        result = myUpdateNrender && myInit && myGame_die;

    }

    return result;
}

static void puun_unload_game() {
    if(GameCodeDLL)
    {
        FreeLibrary(GameCodeDLL);
        myUpdateNrender = 0;
        myInit = 0;
        myGame_die = 0;
    }
}
#include "sdl_common.c"
void sdl_update() {
    SDL_Event event = {0};
    while(SDL_PollEvent(&event)!= 0){
        if(event.type == SDL_QUIT){ myGame_die(game_memory); return; }
        else if(event.type == SDL_MOUSEMOTION) {
            //updateMouse(event.motion.x, event.motion.y);
            //#TODO:110 Screen vs Virtual Space
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
    myUpdateNrender(game_memory);
}

int WinMain(
  _In_ HINSTANCE hInstance,
  _In_ HINSTANCE hPrevInstance,
  _In_ LPSTR     lpCmdLine,
  _In_ int       nCmdShow
        ) {
    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_WM_SetCaption("Puun", NULL);
    if( Mix_OpenAudio( 22050, MIX_DEFAULT_FORMAT, 2, 4096 ) == -1) printf("error initialializ sound\n");
    Mix_Init(MIX_INIT_OGG);
    SDL_SetVideoMode(800, 800, 32, SDL_OPENGL);
    glewInit();
    if(glewInit() != GLEW_OK) {
        fputs("glewInit failed \n", stderr);
    }

    // print out some info about the graphics drivers
    printf("OpenGL version: %s \n", glGetString(GL_VERSION));
    printf("GLSL version: %s \n", glGetString(GL_SHADING_LANGUAGE_VERSION));
    printf("Vendor: %s \n", glGetString(GL_VENDOR));
    printf("Renderer: %s \n", glGetString(GL_RENDERER));

    // make sure OpenGL version 3.2 API is available
    if(!GLEW_VERSION_3_2)
        fputs("OpenGL 3.2 API is not available.\n", stderr);

#if 1
    if(!puun_load_game("game2.dll")) {
        printf("error loading game code");//TODO LOGGER
        return 1;
    }
#endif
    game_memory = calloc(1, 1<<14);
    myInit(game_memory);
    running = true;
    while(running) {
        sdl_update();
        FILETIME newDLLWriteTime = get_last_write_time("game2.dll.new");
        if(CompareFileTime(&newDLLWriteTime, &DLLLastWriteTime) != 0)
        {
            puun_unload_game();
            CopyFile("game2.dll.new", "game2.dll", FALSE);
            puun_load_game("game2.dll");
        }
    }

    return 0;
}

#include "gf/sprite.c"
#include "gf/text.c"
