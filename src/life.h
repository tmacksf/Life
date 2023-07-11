#ifndef LIFE_H
#define LIFE_H

#include "../res/TSL/src/datastructures/vector.h"
#include "raylib.h"
#include <time.h>

// enum for type of grid
enum GridType { TOP, LEFT, RIGHT, BOTTOM };

// grid struct
typedef struct Square {
  int x;
  int y;
  int active;
  int neighbours;
  int around[8]; // indexes of the neighbours
} Square;

// main game loop
int gameLoop();

int init_squares(vector *v, int gap, int size, int screeenWidth,
                 int screenHeight);
void adding_neighbours_side(Square *s, int index, int cols, enum GridType g);

void update_squares(vector *v);
void update_individual_square(vector *v, Square *s);
#endif
