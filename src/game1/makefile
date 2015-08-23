all:
	gcc -Wall -Werror -g -O4 game1.c ../../build/puun.obj -lSDL -lSDL_mixer -lGL -lGLEW -lm -o ../../build/game1
	emcc -Wall -Werror -DJS -O3 game1.c ../../build/puun.o --preload-file ../../assets -o ../../build/game1.html
