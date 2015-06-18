gcc -g -O4 game1.c ../../build/puun.obj -lSDL -lSDL_mixer -lGL -lGLEW -lm -o ../../build/game1
emcc -DJS -O3 game1.c ../../build/puun.o --preload-file ../../assets -o ../../build/game1.html
cd ../../build
./game1
cd ../src/game1
