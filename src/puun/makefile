all:
	gcc -Wall -Wno-enum-compare -Werror -g sdl_puun.c -lSDL -lSDL_mixer -lGL -lGLU -lGLEW -c -o ../../build/puun.obj
	emcc -Wall -Werror -g -DJS sdl_puun.c -o ../../build/puun.o
	gcc -rdynamic -Wall -Wno-enum-compare -Wno-uninitialized -Werror -g sdl_linux_dev_puun.c -ldl -lm -lSDL -lSDL_mixer -lGL -lGLU -lGLEW -o ../../build/dev_puun
