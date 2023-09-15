#ifndef LIFE_H
#define LIFE_H

#include "external/Raylib/raylib.h"
#include "external/TSL/src/datastructures/vector.h"
#include "time.h"

/* Enum for type of grid */
typedef enum GridType { TOP, LEFT, RIGHT, BOTTOM } GridType;

/* Cell struct */
typedef struct Cell {
  int x;
  int y;
  int active;
  int neighbours;
  int around[8]; /* indexes of the neighbours */
} Cell;

int gameLoop();

/* Initialises all cells based on info:
 * gap: gap between cells
 * size: size of the cells
 * screeenWidth
 * screenHeight*/
Vector *initCells(int gap, int size, int screenWidth, int screenHeight);

/* Creates corner squares */
void cornerCells(Vector *v, int rows, int cols, int num);

/* Takes a cell and adds the index of the neightbours to around[] */
void addingNeighboursSide(Cell *cell, int index, int cols, GridType g);

/* Updates all squares for next frame */
void updateCells(Vector *v, int squareNum);

/* takes a cell and updates for the next frame */
int updateIndividualCell(Vector *v, Cell *cell);

#endif
