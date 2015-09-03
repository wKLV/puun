all:
	gcc -Wall -Werror -g -O4 firsty.c ../../build/puun.obj -I../../src -lSDL -lSDL_mixer -lGL -lGLEW -lm -o ../../build/firsty
	emcc -Wall -Werror -DJS firsty.c ../../build/puun.o -I../../src --preload-file ../assets -o ../../build/firsty.html
	gcc -Wall -Werror -g -O4 secondy.c ../../build/puun.obj -I../../src -lSDL -lSDL_mixer -lGL -lGLEW -lm -o ../../build/secondy
	emcc -Wall -Werror -DJS -O3 secondy.c ../../build/puun.o -I../../src --preload-file ../assets -o ../../build/secondy.html
	gcc -Dpuun_INTERNAL -fPIC -Wall -Werror -g -O4 firsty.c -I../../src -lSDL -lSDL_mixer -lGL -lGLEW -lm -shared -o ../../build/firsty.so.new
	gcc -Dpuun_INTERNAL -fPIC -Wall -Werror -g -O4 secondy.c -I../../src -lSDL -lSDL_mixer -lGL -lGLEW -lm -shared -o ../../build/secondy.so.new
	mv -f ../../build/firsty.so.new ../../build/firsty.so
	mv -f ../../build/secondy.so.new ../../build/secondy.so
