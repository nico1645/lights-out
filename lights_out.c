/*
 * file: lights_out.c
 * author: nico bachmann
 * description: simple command line game light out reference (Turning lights out
 * with linear algebra - Marlow Anderson and Todd Feil)
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef struct bit {
  unsigned int x : 1;
} bit;

struct field {
  int width;
  int size;
  bit field[];
};

void print_field(struct field *field);
void set_field(struct field *field, int i, int j);
void set_cross(struct field *field, int i, int j);
void game_loop(struct field *field, int *ticks, int *sol);
void create_init_state(struct field *field, int *ticks, int *sol);
int check_win(struct field *field);
void print_sol(struct field *field, int *sol);

int main(int argc, char **argv) {
  int width = 5;
  int ticks = 7;
  if (argc == 2) {
    width = atoi(argv[1]);
  } else if (argc == 3) {
    width = atoi(argv[1]);
    ticks = atoi(argv[2]);
  }
  if (width == 0) {
    return 1;
  }
  srand((unsigned int)time(NULL));
  int size = width * width;
  struct field *field = malloc(sizeof(struct field) + size * sizeof(bit));
  int sol[size];
  field->width = width;
  field->size = size;
  create_init_state(field, &ticks, sol);
  printf("Lights out: (i)nsert, (q)uit, (s)olution, (r)estart\n");
  print_field(field);
  game_loop(field, &ticks, sol);

  free(field);
  return 0;
}

void game_loop(struct field *field, int *ticks, int *sol) {
  char c;
  int i;
  int j;
  do {
    scanf("%c", &c);
    switch (c) {
    case 'q':
      break;
    case 'i':
      scanf("%d %d", &i, &j);
      if (!(i < field->width && j < field->width) || (i < 0 && j < 0)) {
        break;
      }
      set_cross(field, i, j);
      sol[field->width * i + j] = (sol[field->width * i + j] + 1) % 2;
      print_field(field);
      break;
    case 's':
      print_sol(field, sol);
      break;
    case 'r':
      create_init_state(field, ticks, sol);
      print_field(field);
      break;
    default:
      break;
    }
    while (getchar() != '\n')
      ;
    if (check_win(field)) {
      printf("You won\n");
    }
  } while (c != 'q');
}

void print_sol(struct field *field, int *sol) {
  for (int i = 0; i < field->size; i++) {
    if (sol[i] == 1) {
      printf("(%d %d) ", i / field->width, i % field->width);
    }
  }
  printf("\n");
}

int check_win(struct field *field) {
  for (int i = 0; i < field->size; i++) {
    if (field->field[i].x) {
      return 0;
    }
  }
  return 1;
}

void create_init_state(struct field *field, int *ticks, int *sol) {
  int x;
  int y;
  memset(field->field, 0, field->size * sizeof(*field->field));
  memset(sol, 0, field->size * sizeof(int));
  for (int i = 0; i < *ticks; i++) {
    x = rand() % field->width;
    y = rand() % field->width;
    set_cross(field, x, y);
    sol[field->width * x + y] = (sol[field->width * x + y] + 1) % 2;
  }
}

void set_field(struct field *field, int i, int j) {
  if (field->field[i * field->width + j].x == 0) {
    field->field[i * field->width + j].x = 1;
  } else {
    field->field[i * field->width + j].x = 0;
  }
}

void set_cross(struct field *field, int i, int j) {
  if (i - 1 >= 0) {
    set_field(field, i - 1, j);
  }
  if (i + 1 < field->width) {
    set_field(field, i + 1, j);
  }
  if (j - 1 >= 0) {
    set_field(field, i, j - 1);
  }
  if (j + 1 < field->width) {
    set_field(field, i, j + 1);
  }
  set_field(field, i, j);
}

void print_field(struct field *field) {
  for (unsigned int i = 0; i < (unsigned int) field->size; i++) {
    printf("%d ", field->field[i].x);
    if ((i + 1) % field->width == 0)
      printf("\n");
  }
  printf("\n");
}
