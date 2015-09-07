md build
cl /DWDS /c /Zi /Fobuild/puun src/puun/sdl_puun.c /I "W:\include"

set CommonLinkerFlags= user32.lib gdi32.lib winmm.lib  W:\lib\SDL\SDL.lib W:\lib\GL\glew32.lib W:\lib\SDL\SDL_mixer.lib "C:\Program Files (x86)\Microsoft SDKs\Windows\v7.1A\Lib\x64\OpenGL32.lib"

cl /DWDS /Zi /Febuild/dev_puun.exe /Fobuild/dev_puun.obj src/puun/sdl_wds_dev_puun.c /I "W:\include" /link %CommonLinkerFlags%
