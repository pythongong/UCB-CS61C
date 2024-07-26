#include "compute.h"

static void allocation_failed() {
    fprintf(stderr, "Out of memory.\n");
    exit(1);
}

void swap(int32_t* data, int i, int j) {
  int32_t tmp = data[i];
  data[i] = data[j];
  data[j] = tmp;
}

void flip(matrix_t *origin_matrix) {
  int rows = origin_matrix->rows;
  int cols = origin_matrix->cols;
  int mid_row = rows / 2;
  int mid_col = cols / 2;
  int32_t* data = origin_matrix->data;
  for (int i = mid_row-1, j = rows % 2 == 0 ? mid_row : mid_row + 1; i >= 0 && j < rows; i--, j++) {
    int i_start = i * cols, j_start = j*cols;
    for (int k = 0; k < cols; k++) {
      swap(data, i_start+k, j_start+k);
    }
  }

  for (int i = 0; i < rows; i++) {
    int start_index = i * cols;
    for (int j = mid_col-1, k = cols % 2 == 0 ? mid_col : mid_col+1; j >= 0 && k < cols; j--, k++) {
      swap(data, start_index+j, start_index+k);
    }
  }
  
}

int32_t convolved_result(matrix_t *a_matrix, matrix_t *b_matrix, int top, int bottom, int left, int right) {
  int32_t* a_data = a_matrix->data;
  int32_t* b_data = b_matrix->data;
  int b_index = 0;
  int32_t result = 0;
  for (int i = top; i <= bottom; i++) {
    int start = i * a_matrix->cols;
    for (int j = left; j <= right; j++) {
      result += a_data[start+j] * b_data[b_index++];
    }
  }
  return result;
}

// convolve matrix a and matrix b, and store the resulting matrix in
int convolve(matrix_t *a_matrix, matrix_t *b_matrix, matrix_t **output_matrix) {
  flip(b_matrix);
  int i = 0;
  matrix_t* convolved_matrix = malloc(sizeof(matrix_t));
  if (convolved_matrix == NULL) {
    allocation_failed();
  }
  convolved_matrix->rows = a_matrix->rows - b_matrix->rows + 1;
  convolved_matrix->cols = a_matrix->cols - b_matrix->cols + 1;
  int32_t *data = malloc(convolved_matrix->rows * convolved_matrix->cols * sizeof(int32_t));
  if (data == NULL) {
    allocation_failed();
  }
  
  for (int top = 0, bottom = b_matrix->rows-1; bottom < a_matrix->rows; top++, bottom++) {
    for(int left = 0, right = b_matrix->cols-1; right < a_matrix->cols; left++, right++) {
      data[i] = convolved_result(a_matrix, b_matrix, top, bottom, left, right);
      i++;
    }
  }
  convolved_matrix->data = data;
  *(output_matrix) = convolved_matrix;
  return 0;
}


// Executes a task
int execute_task(task_t *task) {
  matrix_t *a_matrix, *b_matrix, *output_matrix;

  char *a_matrix_path = get_a_matrix_path(task);
  if (read_matrix(a_matrix_path, &a_matrix)) {
    printf("Error reading matrix from %s\n", a_matrix_path);
    return -1;
  }
  free(a_matrix_path);

  char *b_matrix_path = get_b_matrix_path(task);
  if (read_matrix(b_matrix_path, &b_matrix)) {
    printf("Error reading matrix from %s\n", b_matrix_path);
    return -1;
  }
  free(b_matrix_path);

  if (convolve(a_matrix, b_matrix, &output_matrix)) {
    printf("convolve returned a non-zero integer\n");
    return -1;
  }

  char *output_matrix_path = get_output_matrix_path(task);
  if (write_matrix(output_matrix_path, output_matrix)) {
    printf("Error writing matrix to %s\n", output_matrix_path);
    return -1;
  }
  free(output_matrix_path);

  free(a_matrix->data);
  free(b_matrix->data);
  free(output_matrix->data);
  free(a_matrix);
  free(b_matrix);
  free(output_matrix);
  return 0;
}
