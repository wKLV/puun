all:
	gcc -Wall -Werror -g -O4 ../../src/sprites/sprite.c ../../build/puun.obj -lSDL -lSDL_mixer -lGL -lGLEW -lm -o ../../build/sprites
	emcc -Wall -Werror -DJS -O3 ../../src/sprites/sprite.c ../../build/puun.o --preload-file ../../assets -o ../../build/sprites.html
