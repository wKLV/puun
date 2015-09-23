all:
	gcc -Wall -Werror -O4 game1.c ../../build/puun.obj -I/home/cesar/pjs/puun/src -lSDL -lSDL_mixer -lGL -lGLEW -lm -o ../../build/game1
	emcc -Wall -Werror -DJS -O3 game1.c ../../build/puun.o -I../../src --preload-file ../../assets -o ../../build/game1.html
	gcc -Dpuun_INTERNAL -fPIC -Wall -Werror -g game1.c -I/home/cesar/pjs/puun/src -lSDL -lSDL_mixer -lGL -lGLEW -lm -shared -o ../../build/game1.so.new
	mv -f ../../build/game1.so.new ../../build/game1.so
