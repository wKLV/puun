all:
	gcc -Wall -Werror -O4 game2.c ../../build/puun.obj -I../../src/ -lSDL -lSDL_mixer -lGL -lGLEW -lm -o ../../build/game2
	emcc -Wall -Werror -DJS -O3 game2.c ../../build/puun.o -I../../src/ --preload-file ../../assets -o ../../build/game2.html
	gcc -Dpuun_INTERNAL -fPIC -Wall -Werror -O0 -g game2.c -I../../src/ -lSDL -lSDL_mixer -lGL -lGLEW -lm -shared -o ../../build/game2.so.new
	mv -f ../../build/game2.so.new ../../build/game2.so
