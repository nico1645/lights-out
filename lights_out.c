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
void clear(struct field *field, int *sol);
int solve_field(struct field *field, int *sol);

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
  printf("Lights out: (c)reate, (i)nsert, (q)uit, (s)olution, (r)estart\n");
  print_field(field);
  game_loop(field, &ticks, sol);

  free(field);
  return 0;
}

void game_loop(struct field *field, int *ticks, int *sol) {
  char c;
  int i;
  int j;
  int creating = 0;
  int has_sol = 1;
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
      if (creating) {
        set_field(field, i, j);
      } else {
        set_cross(field, i, j);
        sol[field->width * i + j] = (sol[field->width * i + j] + 1) % 2;
      }
      print_field(field);
      break;
    case 's':
      if (has_sol) {
        print_sol(field, sol);
      } else {
        printf("No Solution\n");
      }
      break;
    case 'r':
      create_init_state(field, ticks, sol);
      print_field(field);
      has_sol = 1;
      break;
    case 'c':
      creating = 1;
      printf("Adding single lights through (i)nsert\nEnd it with (d)one\n");
      clear(field, sol);
      print_field(field);
      break;
    case 'd':
      creating = 0;
      has_sol = solve_field(field, sol);
      print_field(field);
      break;
    default:
      break;
    }
    while (getchar() != '\n')
      ;
    if (check_win(field) && creating == 0) {
      printf("You won\n");
    }
  } while (c != 'q');
}

int solve_field(struct field *field, int *sol) {
  int matrix[field->size][field->size + 1];
  memset(matrix, 0, sizeof(int) * field->size * (field->size + 1));
  int x;
  for (int i = 0; i < field->size; i++) {
    for (int j = 0; j < field->size + 1; j++) {
      if (j == field->size) {
        matrix[i][j] = field->field[i].x;
      } else {
        if (i == j) {
          x = i % field->width;
          matrix[i][j] = 1;
          if (x - 1 >= 0) {
            matrix[i - 1][i] = 1;
          }
          if (x + 1 < field->width) {
            matrix[i + 1][j] = 1;
          }
          if (i - field->width >= 0) {
            matrix[i - field->width][j] = 1;
          }
          if (i + field->width < field->size) {
            matrix[i + field->width][j] = 1;
          }
        }
      }
    }
  }

  int n = field->size;
  for (int k = 0; k < n; k++) {
    for (int i = k + 1; i < n; i++) {
      if (matrix[k][k] == 1)
        break;
      if (matrix[k][k] == 0) {
        if (matrix[i][k] == 1) {
          for (int e = k; e <= n; e++) {
            int tmp = matrix[i][e];
            matrix[i][e] = matrix[k][e];
            matrix[k][e] = tmp;
          }
          break;
        }
      }
    }
    for (int i = 0; i < n; i++) {
      if (k != i && matrix[i][k] == 1) {
        for (int j = k; j <= n; j++) {
          matrix[i][j] = matrix[i][j] ^ (matrix[k][j]);
        }
      }
    }
  }

  for (int i = 0; i < field->size; i++) {
    if (matrix[i][i] == 0 && matrix[i][i] != matrix[i][field->size]) return 0;
    sol[i] = matrix[i][field->size];
  }

  return 1;
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

void clear(struct field *field, int *sol) {
  memset(field->field, 0, field->size * sizeof(*field->field));
  memset(sol, 0, field->size * sizeof(int));
}

void create_init_state(struct field *field, int *ticks, int *sol) {
  int x;
  int y;
  clear(field, sol);
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
  for (unsigned int i = 0; i < (unsigned int)field->size; i++) {
    printf("%d ", field->field[i].x);
    if ((i + 1) % field->width == 0)
      printf("\n");
  }
  printf("\n");
}
