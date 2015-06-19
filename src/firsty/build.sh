gcc -g -O4 firsty.c ../../build/puun.obj -lSDL -lSDL_mixer -lGL -lGLEW -lm -o ../../build/firsty
emcc -DJS firsty.c ../../build/puun.o --preload-file ../../assets -o ../../build/firsty.html
gcc -g -O4 secondy.c ../../build/puun.obj -lSDL -lSDL_mixer -lGL -lGLEW -lm -o ../../build/secondy
emcc -DJS -O3 secondy.c ../../build/puun.o --preload-file ../../assets -o ../../build/secondy.html
