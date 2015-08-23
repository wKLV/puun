all:
	gcc -Wall -Werror -Wno-enum-compare -g sdl_puun.c -lSDL -lSDL_mixer -lGL -lGLU -lGLEW -c -o ../../build/puun.obj
	emcc -Wall -Werror -g -DJS sdl_puun.c -o ../../build/puun.o
