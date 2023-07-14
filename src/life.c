#include "life.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>

int init_squares(vector *v, int gap, int size, int screeenWidth,
                 int screenHeight) {
  // TODO: Redo this to make the entire surrounding area a buffer zone so only
  // need to look at square with 8 neighbours
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

  // Corners
  // Top left
  Square *s = vector_at(v, 0);
  s->neighbours = 3;
  s->around[0] = 1;
  s->around[1] = cols;
  s->around[2] = cols + 1;
  // Top right
  s = vector_at(v, cols - 1);
  s->neighbours = 3;
  s->around[0] = cols - 2;
  s->around[1] = cols * 2 - 1;
  s->around[2] = cols * 2 - 2;
  // Bottom left
  s = vector_at(v, num - cols);
  s->neighbours = 3;
  s->around[0] = num - cols + 1;
  s->around[1] = num - cols * 2;
  s->around[2] = num - cols * 2 + 1;
  //  Bottom right
  s = vector_at(v, num - 1);
  s->neighbours = 3;
  s->around[0] = num - 2;
  s->around[1] = num - cols - 1;
  s->around[2] = num - cols - 2;

  // Top row (minus corners)
  for (int c = 1; c < cols - 1; c++) {
    s = vector_at(v, c);
    s->neighbours = 5;
    adding_neighbours_side(s, c, cols, TOP);
  }

  // Bottom row (minus corners)
  for (int c = 1 + (num - cols); c < (num - 1); c++) {
    s = vector_at(v, c);
    s->neighbours = 5;
    adding_neighbours_side(s, c, cols, BOTTOM);
  }

  // others
  int index_adding[] = {
      -1, 1, cols, cols - 1, cols + 1, -cols, -cols + 1, -cols - 1,
  };
  for (int i = cols; i < (num - cols); i++) {
    // left side
    if (i % cols == 0) {
      s = vector_at(v, i);
      s->neighbours = 5;
      adding_neighbours_side(s, i, cols, LEFT);
    }
    // right side
    else if (i % cols == (cols - 1)) {
      s = vector_at(v, i);
      s->neighbours = 5;
      adding_neighbours_side(s, i, cols, RIGHT);
    }
    // middle
    else {
      s = vector_at(v, i);
      s->neighbours = 8;
      for (int j = 0; j < 8; j++) {
        s->around[j] = i + index_adding[j];
      }
    }
  }

  return 0;
}

void adding_neighbours_side(Square *s, int index, int cols, enum GridType g) {
  int top_side[] = {-1, 1, cols, cols + 1, cols - 1};
  // TODO: The rest of these
  int left_side[] = {1, cols, cols + 1, -cols, -cols + 1};
  int right_side[] = {-1, cols, cols - 1, -cols, -cols - 1};
  int bottom_side[] = {-1, 1, -cols, -cols + 1, -cols - 1};
  switch (g) {
  case TOP:
    for (int i = 0; i < 5; i++) {
      s->around[i] = index + top_side[i];
    }
    break;
  case LEFT:
    for (int i = 0; i < 5; i++) {
      s->around[i] = index + left_side[i];
    }
    break;
  case RIGHT:
    for (int i = 0; i < 5; i++) {
      s->around[i] = index + right_side[i];
    }
    break;
  case BOTTOM:
    for (int i = 0; i < 5; i++) {
      s->around[i] = index + bottom_side[i];
    }
    break;
  }
}

int update_individual_square(vector *v, Square *s) {
  int activeAround = 0;
  for (int i = 0; i < s->neighbours; i++) {
    Square *neighbour = vector_at(v, s->around[i]);
    activeAround += neighbour->active;
  }

  if (s->active) {
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

void update_squares(vector *v, int squareNum) {
  int allUpdates[squareNum];
  for (int i = 0; i < v->size; i++) {
    Square *s = vector_at(v, i);
    allUpdates[i] = update_individual_square(v, s);
  }

  for (int i = 0; i < v->size; i++) {
    Square *s = vector_at(v, i);
    s->active = allUpdates[i];
  }
}

// Temp function randomise
void randomise_active(vector *v) {
  srand(time(0));
  for (int i = 0; i < v->size; i++) {
    if ((rand() % 2) == 1) {
      Square *s = vector_at(v, i);
      s->active = 1;
    }
  }
}

int gameLoop() {
  // variables
  const int width = 1200;
  const int height = 800;
  const int gap = 2;
  const int size = 20;

  vector *v = vector_init(sizeof(Square));
  init_squares(v, gap, 20, width, height);

  InitWindow(width, height, "test");

  // game control
  int space_pressed = 1;
  SetTargetFPS(60);
  randomise_active(v);

  int frame_skip = 5;
  int current_frame_skip = frame_skip;

  while (!WindowShouldClose()) {
    BeginDrawing();

    ClearBackground(RAYWHITE);

    for (int i = 0; i < v->size; i++) {
      Square *s = vector_at(v, i);
      Color c;
      if (s->active)
        c = DARKBLUE;
      else
        c = LIGHTGRAY;
      DrawRectangle(s->x, s->y, size, size, c);
    }

    if (IsKeyPressed(KEY_SPACE)) {
      space_pressed = 1 - space_pressed;
    }

    if (!space_pressed && !current_frame_skip) {
      update_squares(v, v->size);
      current_frame_skip = frame_skip;
    } else if (!space_pressed && current_frame_skip) {
      current_frame_skip -= 1;
    }

    if (IsKeyPressed(KEY_LEFT)) {
      frame_skip += 1;
    }

    if (IsKeyPressed(KEY_RIGHT)) {
      if (frame_skip)
        frame_skip -= 1;
    }

    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
      // Mouse button down to change the value of a square
      // run through every square to see if its correct (not going to be pressed
      // all the time so no need for intense optimisation)
      Vector2 pos = GetMousePosition();
      int x = (int)pos.x;
      int y = (int)pos.y;
      // need to check if it is in the bounds of a given square
      for (int i = 0; i < v->size; i++) {
        Square *s = vector_at(v, i);
        if ((x >= s->x) && (x <= s->x + size) && (y >= s->y) &&
            (y <= s->y + size)) {
          s->active = 1 - s->active;
          break;
        }
      }
    }

    EndDrawing();
  }

  CloseWindow();

  return 0;
}
