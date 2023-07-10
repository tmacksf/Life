#include "life.h"

int init_squares(vector *v, int gap, int size, int screeenWidth,
                 int screenHeight) {
  int filled = 0;
  int totalSize = size + gap;
  int y = gap;
  int num = 0;
  int rows = 0;
  while (!filled) {
    rows++;
    for (int x = gap; x < screeenWidth; x += totalSize) {
      Square s = {x, y, 0, 0};
      vector_add(v, &s);
      num++;
      // v->add(v, &s);
    }
    y += totalSize;
    if (y > screenHeight) {
      filled = 1;
    }
  }
  printf("Rows: %d, Total squares %d\n", rows, num);

  int cols = num / rows;
  // hard coding cuz im being lazy
  // setting neighbour pointers
  // first row
  Square *s = vector_at(v, 0);
  s->neighbours = 3;

  for (int c = 1; c < cols - 1; c++) {
    s = vector_at(v, c);
    s->neighbours = 5;
  }
  s = vector_at(v, cols - 1);
  s->neighbours = 3;

  // others
  for (int i = cols; i < (num - cols); i++) {
    // left side
    if (i % cols == 0) {
      s = vector_at(v, i);
      s->neighbours = 5;
    }
    // right side
    else if (i % cols == (cols - 1)) {

      s = vector_at(v, i);
      s->neighbours = 5;
    }
    // middle
    else {
      s = vector_at(v, i);
      s->neighbours = 8;
    }
  }

  // last row
  s = vector_at(v, num - cols);
  s->neighbours = 3;
  for (int c = 1 + (num - cols); c < (num - 1); c++) {
    s = vector_at(v, c);
    s->neighbours = 5;
  }
  s = vector_at(v, num - 1);
  s->neighbours = 3;

  return 0;
}

void adding_neighbours(Square *s, int startingIndex) {
  for (int i = 0; i < s->neighbours; i++) {
  }
}

void update_individual_square(Square *s, int activeAround) {
  if (s->active) {
    switch (activeAround) {
    case 2:
      s->active = 1;
    case 3:
      s->active = 1;
    default:
      s->active = 0;
    }
  } else {
    switch (activeAround) {
    case 3:
      s->active = 1;
    default:
      s->active = 0;
    }
  }
}

void update_squares(vector *v) {
  for (int i = 0; i < v->size; i++) {
    Square *s = vector_at(v, i);
  }
}

int gameLoop() {
  // variables
  int width = 1200;
  int height = 800;
  const int gap = 2;
  const int size = 20;

  vector *v = vector_init(sizeof(Square));
  init_squares(v, gap, 20, width, height);
  InitWindow(width, height, "test");

  // game control
  int space_pressed = 1;
  SetTargetFPS(30);

  while (!WindowShouldClose()) {
    BeginDrawing();

    ClearBackground(RAYWHITE);

    // first 2 give location, 2nd 2 give size
    //  DrawRectangle(2, 2, 20, 20, BLUE);
    for (int i = 0; i < v->size; i++) {
      Square *s = vector_at(v, i);
      Color c;
      if (s->neighbours == 8) {
        c = BLUE;
      }
      if (s->neighbours == 5) {
        c = GREEN;
      }
      if (s->neighbours == 3) {
        c = VIOLET;
      }
      DrawRectangle(s->x, s->y, size, size, c);

      /*
      if (s->active) {
        DrawRectangle(s->x, s->y, size, size, BLUE);
      } else {
        DrawRectangle(s->x, s->y, size, size, WHITE);
      }
      */
    }

    if (IsKeyPressed(KEY_SPACE)) {
      space_pressed = 1 - space_pressed;
    }

    if (!space_pressed) {
      update_squares(v);
    }

    EndDrawing();
  }

  CloseWindow();

  return 0;
}
