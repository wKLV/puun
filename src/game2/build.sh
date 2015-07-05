gcc -g -O4 game2.c ../../build/puun.obj -lSDL -lSDL_mixer -lGL -lGLEW -lm -o ../../build/game2
emcc -DJS -O3 game2.c ../../build/puun.o --preload-file ../../assets -o ../../build/game2.html
cd ../../build
./game2
cd ../src/game2
