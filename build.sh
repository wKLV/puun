if [ ! -d "build" ]; then
    mkdir build
fi

gcc -O4 src/puun/sdl_puun.c -lSDL -lSDL_mixer -lGL -lGLEW -c -o build/puun.obj
emcc -DJS -O3 src/puun/sdl_puun.c -o build/puun.o

