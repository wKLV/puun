md build
cl -DWDS -Zi -Febuild/firsty.exe src/firsty/sdl_firsty.c OpenGL32.lib /I "W:\include" /link W:\lib\SDL\SDL.lib W:\lib\GL\glew32.lib W:\lib\SDL\SDL_mixer.lib
cl -DWDS -Zi -Febuild/secondy.exe src/firsty/sdl_secondy.c OpenGL32.lib /I "W:\include" /link W:\lib\SDL\SDL.lib W:\lib\GL\glew32.lib W:\lib\SDL\SDL_mixer.lib
cl -DWDS -Zi -Febuild/textytest.exe src/texttest/sdl_tt.c OpenGL32.lib /I "W:\include" /link W:\lib\SDL\SDL.lib W:\lib\GL\glew32.lib W:\lib\SDL\SDL_mixer.lib
cl -DWDS -Zi -Febuild/sprite.exe src/sprites/sdl_sprites.c OpenGL32.lib /I "W:\include" /link W:\lib\SDL\SDL.lib W:\lib\GL\glew32.lib W:\lib\SDL\SDL_mixer.lib
