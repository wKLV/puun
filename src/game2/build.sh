ERRORS=./../errors.err
if [ -d $ERRORS ]; then
    rm $ERRORS
fi
gcc -Wall -Werror -g -O4 game2.c ../../build/puun.obj -lSDL -lSDL_mixer -lGL -lGLEW -lm -o ../../build/game2 2> $ERRORS
emcc -Wall -Werror -DJS -O3 game2.c ../../build/puun.o --preload-file ../../assets -o ../../build/game2.html 2> $ERRORS
 
#if [ $? -eq 0 ]; then
#    cd ../../build
#    ./game2 &
#    cd ../src/game2
#fi
