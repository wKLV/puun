all:
	gcc -Wall -Werror -O4 main.c ../../build/puun.obj -I/home/cesar/pjs/puun/src -lSDL -lSDL_mixer -lGL -lGLEW -lm -o ../../build/mesh
	emcc -Wall -Werror -DJS -O3 main.c ../../build/puun.o -I../../src --preload-file ../../assets -o ../../build/mesh.html
	gcc -Dpuun_INTERNAL -fPIC -Wall -Werror -g main.c -I/home/cesar/pjs/puun/src -lSDL -lSDL_mixer -lGL -lGLEW -lm -shared -o ../../build/mesh.so.new
	mv -f ../../build/mesh.so.new ../../build/mesh.so
