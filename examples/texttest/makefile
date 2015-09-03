all:
	gcc -Wall -Werror -g -O4 tt.c ../../build/puun.obj -I../../src/ -lSDL -lSDL_mixer -lGL -lGLEW -lm -o ../../build/textytest
	emcc -Wall -Werror -DJS -O3 tt.c ../../build/puun.o -I../../src/ --preload-file ../../assets -o ../../build/textytest.html
	gcc -Dpuun_INTERNAL -fPIC -Wall -Werror -g -O4 tt.c -I../../src/ -lSDL -lSDL_mixer -lGL -lGLEW -lm -shared -o ../../build/textytest.so.new
	mv -f ../../build/textytest.so.new ../../build/textytest.so
