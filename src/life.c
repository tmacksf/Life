#include "life.h"
#include "external/TSL/src/datastructures/vector.h"

Vector *initCells(int gap, int size, int screenWidth, int screenHeight) {
  Vector *v = vector_init(sizeof(Cell));
  int totalSize = size + gap;
  int y = gap;
  int num = 0;
  int rows = 0;

  /* counting size */
  int filled = 0;
  while (!filled) {
    rows++;
    for (int x = gap; x < screenWidth; x += totalSize) {
      Cell cell = {.x = x, .y = y, .active = 0, .neighbours = 0};
      vector_add(v, &cell);
      num++;
    }
    y += totalSize;
    if (y > screenHeight) {
      filled = 1;
    }
  }
  int cols = num / rows;
  printf("Rows: %d, Cols: %d, Total squares %d, Cell Size: %d, Gap Size: %d, "
         "Total "
         "Size %d\n",
         rows, cols, num, size, gap, totalSize);

  /* possible directions of neightbours */
  int directions[8] = {
      -1, 1, cols, cols - 1, cols + 1, -cols, -cols + 1, -cols - 1,
  };
  for (int i = 0; i < num; i++) {
    /* For every cell we go over potential neighbours to see if they should be
     * added Have 8 neighbours max */
    Cell *c = vector_at(v, i);
    /* used within the cell to assign neighbourIndex */
    int index = 0;
    for (int n = 0; n < 8; n++) {
      /* check bounds of additions */
      int neighbourIndex = i + directions[n];
      /* Dealing with top and bottom */
      if (neighbourIndex > num || neighbourIndex < 0)
        continue;

      /* Dealing with left side */
      if (i % cols == 0 && (n == 0 || n == 3 || n == 7))
        continue;

      /* Dealing with right side */
      if (i % cols == (cols - 1) && (n == 1 || n == 4 || n == 6))
        continue;

      c->around[index] = i + directions[n];
      index++;
    }
    // c->neighbours = index;
    c->neighbours = index;
  }
  return v;
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
  const int width = 800;
  const int height = 600;
  const int gap = 1;
  const int size = 10;

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
