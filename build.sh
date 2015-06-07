if [ ! -d "build" ]; then
    mkdir build
fi

gcc -O4 src/puun/sdl_puun.c -lSDL -lSDL_mixer -lGL -lGLEW -c -o build/puun.obj
emcc -DJS -O3 src/puun/sdl_puun.c -o build/puun.o

gcc -g -O4 src/firsty/firsty.c build/puun.obj -lSDL -lSDL_mixer -lGL -lGLEW -lm -o build/firsty
emcc -DJS src/firsty/firsty.c build/puun.o --preload-file assets -o build/firsty.html
gcc -g -O4 src/firsty/secondy.c build/puun.obj -lSDL -lSDL_mixer -lGL -lGLEW -lm -o build/secondy
emcc -DJS -O3 src/firsty/secondy.c build/puun.o --preload-file assets -o build/secondy.html

gcc -g -O4 src/texttest/tt.c build/puun.obj -lSDL -lSDL_mixer -lGL -lGLEW -lm -o build/textytest
emcc -DJS -O3 src/texttest/tt.c build/puun.o --preload-file assets -o build/textytest.html
gcc -g -O4 src/sprites/sprite.c build/puun.obj -lSDL -lSDL_mixer -lGL -lGLEW -lm -o build/sprites
emcc -DJS -O3 src/sprites/sprite.c build/puun.o --preload-file assets -o build/sprites.html
