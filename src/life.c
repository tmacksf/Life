#include "life.h"
#include "external/TSL/src/datastructures/vector.h"

Vector *initCells(int gap, int size, int screenWidth, int screenHeight) {
  Vector *v = vector_init(sizeof(Cell));
  /* TODO: Redo this to make the entire surrounding area a buffer zone so only
   * need to look at square with 8 neighbours */
  int filled = 0;
  int totalSize = size + gap;
  int y = gap;
  int num = 0;
  int rows = 0;

  /* counting size */
  while (!filled) {
    rows++;
    for (int x = gap; x < screenWidth; x += totalSize) {
      Cell cell = {x, y, 0, 0};
      vector_add(v, &cell);
      num++;
    }
    y += totalSize;
    if (y > screenHeight) {
      filled = 1;
    }
  }
  printf("Rows: %d, Total squares %d, Cell Size: %d, Gap Size: %d, Total "
         "Size %d\n",
         rows, num, size, gap, totalSize);

  int cols = num / rows;
  cornerCells(v, rows, cols, num);
  Cell *cell;

  /* Top row (minus corners) */
  for (int c = 1; c < cols - 1; c++) {
    cell = vector_at(v, c);
    cell->neighbours = 5;
    addingNeighboursSide(cell, c, cols, TOP);
  }

  /* Bottom row (minus corners) */
  for (int c = 1 + (num - cols); c < (num - 1); c++) {
    cell = vector_at(v, c);
    cell->neighbours = 5;
    addingNeighboursSide(cell, c, cols, BOTTOM);
  }

  int indexAdding[] = {
      -1, 1, cols, cols - 1, cols + 1, -cols, -cols + 1, -cols - 1,
  };
  for (int i = cols; i < (num - cols); i++) {
    /* Left side */
    if (i % cols == 0) {
      cell = vector_at(v, i);
      cell->neighbours = 5;
      addingNeighboursSide(cell, i, cols, LEFT);
    }
    /* right side */
    else if (i % cols == (cols - 1)) {
      cell = vector_at(v, i);
      cell->neighbours = 5;
      addingNeighboursSide(cell, i, cols, RIGHT);
    }
    /* middle */
    else {
      cell = vector_at(v, i);
      cell->neighbours = 8;
      for (int j = 0; j < 8; j++) {
        cell->around[j] = i + indexAdding[j];
      }
    }
  }

  return v;
}

void cornerCells(Vector *v, int rows, int cols, int num) {
  /* Top left */
  Cell *cell = vector_at(v, 0);
  cell->neighbours = 3;
  cell->around[0] = 1;
  cell->around[1] = cols;
  cell->around[2] = cols + 1;
  /* Top right */
  cell = vector_at(v, cols - 1);
  cell->neighbours = 3;
  cell->around[0] = cols - 2;
  cell->around[1] = cols * 2 - 1;
  cell->around[2] = cols * 2 - 2;
  /* Bottom left */
  cell = vector_at(v, num - cols);
  cell->neighbours = 3;
  cell->around[0] = num - cols + 1;
  cell->around[1] = num - cols * 2;
  cell->around[2] = num - cols * 2 + 1;
  /*  Bottom right */
  cell = vector_at(v, num - 1);
  cell->neighbours = 3;
  cell->around[0] = num - 2;
  cell->around[1] = num - cols - 1;
  cell->around[2] = num - cols - 2;
}

void addingNeighboursSide(Cell *cell, int index, int cols, enum GridType g) {
  int topSide[] = {-1, 1, cols, cols + 1, cols - 1};
  /* TODO: The rest of these */
  int leftSide[] = {1, cols, cols + 1, -cols, -cols + 1};
  int rightSide[] = {-1, cols, cols - 1, -cols, -cols - 1};
  int bottomSide[] = {-1, 1, -cols, -cols + 1, -cols - 1};
  switch (g) {
  case TOP:
    for (int i = 0; i < 5; i++) {
      cell->around[i] = index + topSide[i];
    }
    break;
  case LEFT:
    for (int i = 0; i < 5; i++) {
      cell->around[i] = index + leftSide[i];
    }
    break;
  case RIGHT:
    for (int i = 0; i < 5; i++) {
      cell->around[i] = index + rightSide[i];
    }
    break;
  case BOTTOM:
    for (int i = 0; i < 5; i++) {
      cell->around[i] = index + bottomSide[i];
    }
    break;
  }
}

int updateIndividualCell(Vector *v, Cell *cell) {
  int activeAround = 0;
  for (int i = 0; i < cell->neighbours; i++) {
    Cell *neighbour = vector_at(v, cell->around[i]);
    activeAround += neighbour->active;
  }

  if (cell->active) {
    switch (activeAround) {
    case 2:
      return 1;
    case 3:
      return 1;
    default:
      return 0;
    }
  } else {
    switch (activeAround) {
    case 3:
      return 1;
    default:
      return 0;
    }
  }
}

void updateCells(Vector *v, int squareNum) {
  int allUpdates[squareNum];
  for (int i = 0; i < v->size; i++) {
    Cell *cell = vector_at(v, i);
    allUpdates[i] = updateIndividualCell(v, cell);
  }

  for (int i = 0; i < v->size; i++) {
    Cell *cell = vector_at(v, i);
    cell->active = allUpdates[i];
  }
}

/* Temp function randomise */
void randomiseActive(Vector *v) {
  srand(time(0));
  for (int i = 0; i < v->size; i++) {
    if ((rand() % 2) == 1) {
      Cell *cell = vector_at(v, i);
      cell->active = 1;
    }
  }
}

int gameLoop() {
  /* variables */
  const int width = 1200;
  const int height = 800;
  const int gap = 1;
  const int size = 5;

  Vector *squareVector = initCells(gap, size, width, height);

  InitWindow(width, height, "test");

  /* game control */
  int spacePressed = 1;
  SetTargetFPS(60);
  randomiseActive(squareVector);

  int frameSkip = 5;
  int currentFrameSkip = frameSkip;

  while (!WindowShouldClose()) {
    BeginDrawing();

    ClearBackground(RAYWHITE);

    for (int i = 0; i < squareVector->size; i++) {
      /* TODO: Check if a square is in the frame
       * if in frame -> render, else -> continue
      if (!s->inframe)
        continue;
      */
      Cell *cell = vector_at(squareVector, i);
      Color c;
      if (cell->active)
        c = DARKBLUE;
      else
        c = LIGHTGRAY;
      DrawRectangle(cell->x, cell->y, size, size, c);
    }

    if (IsKeyPressed(KEY_SPACE)) {
      spacePressed = 1 - spacePressed;
    }

    if (!spacePressed && !currentFrameSkip) {
      updateCells(squareVector, squareVector->size);
      currentFrameSkip = frameSkip;
    } else if (!spacePressed && currentFrameSkip) {
      currentFrameSkip -= 1;
    }

    if (IsKeyPressed(KEY_LEFT)) {
      frameSkip += 1;
    }

    if (IsKeyPressed(KEY_RIGHT)) {
      if (frameSkip)
        frameSkip -= 1;
    }

    /* if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) { */
    if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
      /* Mouse button down to change the value of a square run through every
       * square to see if its correct (not going to be pressed all the time so
       * no need for intense optimisation) */
      Vector2 pos = GetMousePosition();
      int x = (int)pos.x;
      int y = (int)pos.y;
      /* need to check if it is in the bounds of a given square */
      for (int i = 0; i < squareVector->size; i++) {
        Cell *cell = vector_at(squareVector, i);
        if ((x >= cell->x) && (x <= cell->x + size) && (y >= cell->y) &&
            (y <= cell->y + size)) {
          cell->active = 1 - cell->active;
          break;
        }
      }
    }

    EndDrawing();
  }

  CloseWindow();
  vector_delete(squareVector);

  return 0;
}
