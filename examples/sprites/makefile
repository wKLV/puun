all:
	gcc -Wall -Werror -g -O4 sprite.c -I/home/cesar/pjs/puun/src ../../build/puun.obj -lSDL -lSDL_mixer -lGL -lGLEW -lm -o ../../build/sprites
	emcc -Wall -Werror -DJS -O3 sprites/sprite.c  -I/home/cesar/pjs/puun/src ../../build/puun.o --preload-file ../../assets -o ../../build/sprites.html
