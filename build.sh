if [ ! -d "build" ]; then
    mkdir build
fi

gcc -g src/puun/sdl_puun.c -lSDL -lSDL_mixer -lGL -lGLU -lGLEW -c -o build/puun.obj
emcc -g -DJS -O3 src/puun/sdl_puun.c -o build/puun.o

