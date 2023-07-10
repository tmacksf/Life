eval cc main.c life.c ../res/TSL/src/datastructures/vector.c -framework IOKit -framework Cocoa -framework OpenGL $(pkg-config --libs --cflags raylib) -o ../bin/life
