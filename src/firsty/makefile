all:
	gcc -Wall -Werror -g -O4 firsty.c ../../build/puun.obj -lSDL -lSDL_mixer -lGL -lGLEW -lm -o ../../build/firsty
	emcc -Wall -Werror -DJS firsty.c ../../build/puun.o --preload-file ../../assets -o ../../build/firsty.html
	gcc -Wall -Werror -g -O4 secondy.c ../../build/puun.obj -lSDL -lSDL_mixer -lGL -lGLEW -lm -o ../../build/secondy
	emcc -Wall -Werror -DJS -O3 secondy.c ../../build/puun.o --preload-file ../../assets -o ../../build/secondy.html
	gcc -Dpuun_INTERNAL -fPIC -Wall -Werror -g -O4 firsty.c -lSDL -lSDL_mixer -lGL -lGLEW -lm -shared -o ../../build/firsty.so
	gcc -Dpuun_INTERNAL -fPIC -Wall -Werror -g -O4 secondy.c -lSDL -lSDL_mixer -lGL -lGLEW -lm -shared -o ../../build/secondy.so
