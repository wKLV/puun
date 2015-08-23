gcc -g -O4 tt.c ../../build/puun.obj -lSDL -lSDL_mixer -lGL -lGLU -lglut -lGLEW -lm -o ../../build/textytest
#emcc -DJS -O3 tt.c ../../build/puun.o --preload-file ../../assets -o ../../build/textytest.html
