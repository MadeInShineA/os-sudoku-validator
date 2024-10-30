#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#define NUMBER_THREADS 11
#define NUMBER_OF_ROWS 9
#define NUMBER_OF_COLUMNS 9

/* structure for passing data to threads */
typedef struct {
  int row;
  int column;
  int thread_number;
} parameters;

parameters parameters_array[NUMBER_THREADS];
bool is_sudoku_valid_array[NUMBER_THREADS] = {
    true, true, true, true, true, true, true, true, true, true, true};

int sudoku[9][9] = {{1, 2, 6, 7, 3, 5, 8, 4, 9}, {5, 9, 3, 4, 8, 1, 6, 7, 2},
                    {8, 7, 4, 6, 9, 2, 5, 1, 3}, {3, 4, 2, 9, 1, 8, 7, 6, 5},
                    {7, 8, 5, 2, 6, 3, 1, 9, 4}, {6, 1, 9, 5, 4, 7, 3, 2, 8},
                    {2, 5, 8, 1, 7, 4, 9, 3, 6}, {4, 6, 7, 3, 5, 9, 2, 8, 1},
                    {9, 3, 1, 8, 2, 6, 4, 5, 7}};

void *validate_rows(void *args) {
  parameters *data = (parameters *)args;
  int thread_number = data->thread_number;

  for (int row = 0; row < NUMBER_OF_ROWS; row++) {
    bool numbers_bool_array[NUMBER_OF_COLUMNS] = {
        false, false, false, false, false, false, false, false, false};
    for (int column = 0; column < NUMBER_OF_COLUMNS; column++) {
      int current_number = sudoku[row][column];
      bool is_number_in_array = numbers_bool_array[current_number - 1];
      if (!is_number_in_array) {
        numbers_bool_array[current_number - 1] = true;
      } else {
        is_sudoku_valid_array[thread_number] = false;
        pthread_exit(0);
      }
    }
  }
  pthread_exit(0);
}

void *validate_columns(void *args) {
  parameters *data = (parameters *)args;
  int thread_number = data->thread_number;

  for (int column = 0; column < NUMBER_OF_COLUMNS; column++) {
    bool numbers_bool_array[NUMBER_OF_ROWS] = {
        false, false, false, false, false, false, false, false, false};
    for (int row = 0; row < NUMBER_OF_ROWS; row++) {
      int current_number = sudoku[row][column];
      bool is_number_in_array = numbers_bool_array[current_number - 1];
      if (!is_number_in_array) {
        numbers_bool_array[current_number - 1] = true;
      } else {
        is_sudoku_valid_array[thread_number] = false;
        pthread_exit(0);
      }
    }
  }
  pthread_exit(0);
}

void *validate_square(void *args) {
  parameters *data = (parameters *)args;
  bool numbers_bool_array[NUMBER_OF_COLUMNS] = {
      false, false, false, false, false, false, false, false, false};
  int row = data->row;
  int column = data->column;
  int thread_number = data->thread_number;

  for (int i = row; i < row + 3; i++) {
    for (int j = column; j < column + 3; j++) {
      numbers_bool_array[sudoku[i][j] - 1] = true;
    }
  }

  for (int i = 0; i < NUMBER_OF_COLUMNS; i++) {
    if (numbers_bool_array[i] == false) {
      is_sudoku_valid_array[thread_number] = false;
      break;
    }
  }
  pthread_exit(0);
}

int main() {
  pthread_t threads[NUMBER_THREADS];

  for (int i = 0; i < NUMBER_THREADS; i++) {
    parameters_array[i].row = 0;
    parameters_array[i].column = 0;
    parameters_array[i].thread_number = i;
    if (i < 9) {
      parameters_array[i].row = (i / 3) * 3;
      parameters_array[i].column = (i * 3) % 9;
      if (pthread_create(&threads[i], NULL, &validate_square,
                         (void *)&parameters_array[i]) != 0) {
        printf("Failed to create thread");
        exit(0);
      }
    } else if (i == 9) {
      if (pthread_create(&threads[i], NULL, &validate_rows,
                         (void *)&parameters_array[i]) != 0) {
        printf("Failed to create thread");
        exit(0);
      }
    } else {
      if (pthread_create(&threads[i], NULL, &validate_columns,
                         (void *)&parameters_array[i]) != 0) {
        printf("Failed to create thread");
        exit(0);
      }
    }
  }

  for (int i = 0; i < NUMBER_THREADS; i++) {
    if (pthread_join(threads[i], NULL) != 0) {
      printf("Failed to join thread");
      exit(0);
    }
  }

  bool is_sudoku_valid = true;
  for (int i = 0; i < NUMBER_THREADS; i++) {
    if (!is_sudoku_valid_array[i]) {
      is_sudoku_valid = false;
      break;
    }
  }

  if (is_sudoku_valid) {
    printf("The sudoku is valid");
  } else {
    printf("The sudoku is not valid");
  }

  return 0;
}
