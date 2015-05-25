#gcc -g -O4 src/firsty/sdl_firsty.c -lSDL -lGL -lGLEW -lm -o build/firsty
#emcc -DJS src/firsty/sdl_firsty.c --preload-file assets -o build/firsty.html
#gcc -g -O4 src/firsty/sdl_secondy.c -lSDL -lGL -lGLEW -lm -o build/secondy
#emcc -DJS src/firsty/sdl_secondy.c --preload-file assets -o build/secondy.html
#
gcc -g src/texttest/sdl_tt.c -lSDL -lGL -lGLEW -lm -o build/textytest
#emcc -DJS -g src/texttest/sdl_tt.c --preload-file assets -o build/textytest.html
