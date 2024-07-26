#include <omp.h>
#include <x86intrin.h>

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

// void init_overlap(int32_t* overlap_matrix, ) {

// }

int32_t convolved_result(matrix_t *a_matrix, matrix_t *b_matrix, int top, int bottom, int left, int right) {
  int tmp[8];
  int size = b_matrix->rows * b_matrix->cols;
  int32_t overlap[size];
  int dlp_limit = (size / 8) * 8;
  int result = 0;
  int index = 0;
  __m256i sum_vector = _mm256_setzero_si256();
  int32_t* a_data = a_matrix->data;
  int32_t* b_data = b_matrix->data;

  for (int i = top; i <= bottom; i++) {
    int start = i * a_matrix->cols;
    int j = left;
    for(; j <= right; j++) {
      overlap[index++] = a_data[start + j];
    }

    // int dlp_limit = (right / 8) * 8;
    // for (; j < dlp_limit; j+=8) {
    //   __m256i a_vector = _mm256_loadu_si256((__m256i *) a_data+start+j);
    //   __m256i b_vector = _mm256_loadu_si256((__m256i *) b_data+start+j);
    //   __m256i product_vector = _mm256_mul_epi32(a_vector, b_vector);
    //   sum_vector = _mm256_add_epi32(sum_vector, product_vector);
    // }
    // for(; j <= right; j++) {
    //   result += a_data[start+j]*b_data[start+j];
    // }
    
  }
  
  for (index = 0; index < dlp_limit; index += 8) {
    __m256i a_vector = _mm256_loadu_si256((__m256i *) overlap+index);
    __m256i b_vector = _mm256_loadu_si256((__m256i *) b_data+index);
    __m256i product_vector = _mm256_mullo_epi32(a_vector, b_vector);
    // int32_t* values = (int32_t*)&product_vector;
    // for (int i = 0; i < 8; i++) { // __m256i contains 8 int32_t values
    //     printf("%d,", values[i]);
    // }
    // printf("product\n");
    sum_vector = _mm256_add_epi32(sum_vector, product_vector);
    // values = (int32_t*)&sum_vector;
    // for (int i = 0; i < 8; i++) { // __m256i contains 8 int32_t values
    //     printf("%d,", values[i]);
    // }
    // printf("sum\n");
  }

  _mm256_storeu_si256((__m256i *) tmp, sum_vector);
  for (int i = 0; i < 8; i++) {
    result += tmp[i];
  }
  

  for(; index < size; index++) {
    result += overlap[index] * b_data[index];
  }
  return result;
}

// convolve matrix a and matrix b, and store the resulting matrix in
int convolve(matrix_t *a_matrix, matrix_t *b_matrix, matrix_t **output_matrix) {
  int a_rows = a_matrix->rows;
  int b_cols = b_matrix->cols;
  flip(b_matrix);
  int i = 0;
  matrix_t* convolved_matrix = malloc(sizeof(matrix_t));
  if (convolved_matrix == NULL) {
    allocation_failed();
  }
  convolved_matrix->rows = a_rows - b_matrix->rows + 1;
  convolved_matrix->cols = a_matrix->cols - b_cols + 1;
  int32_t *data = malloc(convolved_matrix->rows * convolved_matrix->cols * sizeof(int32_t));
  if (data == NULL) {
    allocation_failed();
  }
  
  for (int top = 0, bottom = b_matrix->rows-1; bottom < a_rows; top++, bottom++) {
    for(int left = 0, right = b_cols-1; right < a_matrix->cols; left++, right++) {
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

// int main(int argc, char const *argv[])
// {

//   matrix_t a_matrix;
//   int32_t a_data[] = {7, 4, 9, 3, 5, 3, 7, 5, 9, 7, 2, 4, 9, 2, 10};
//   a_matrix.rows = 5;
//   a_matrix.cols = 3;
//   a_matrix.data = a_data;

//   matrix_t b_matrix;
//   int32_t b_data[] = {9, 10, 5, 2, 4, 7, 8, 3};
//   b_matrix.rows = 4;
//   b_matrix.cols = 2;
//   b_matrix.data = b_data;

//   matrix_t *output_matrix;
//   convolve(&a_matrix, &b_matrix, &output_matrix);
//   int32_t* out = output_matrix->data;
//   for (int i = 0; i < output_matrix->rows * output_matrix->cols; i++) {
//     if((i+1) % output_matrix->cols == 0) {
//       printf("%d\n", out[i]);
//     } else {
//       printf("%d,", out[i]);

//     }
//   }
//   free(output_matrix->data);
//   free(output_matrix);


  // matrix_t a_matrix;
  // int32_t a_data[] = {1, 1, 1, 1, 0, 2, 1, 2, 2};
  // a_matrix.rows = 3;
  // a_matrix.cols = 3;
  // a_matrix.data = a_data;

  // matrix_t b_matrix;
  // int32_t b_data[] = {2, 1, 1, 0};
  // b_matrix.rows = 2;
  // b_matrix.cols = 2;
  // b_matrix.data = b_data;

  // matrix_t *output_matrix;
  // convolve(&a_matrix, &b_matrix, &output_matrix);
  // int32_t* out = output_matrix->data;
  // for (int i = 0; i < output_matrix->rows * output_matrix->cols; i++) {
  //   if((i+1) % output_matrix->cols == 0) {
  //     printf("%d\n", out[i]);
  //   } else {
  //     printf("%d,", out[i]);

  //   }
  // }
  // free(output_matrix->data);
  // free(output_matrix);

  // flip(&b_matrix);
  // for (int i = 0; i < b_matrix.rows * b_matrix.cols; i++) {
  //   if((i+1) % b_matrix.cols == 0) {
  //     printf("%d\n", b_data[i]);
  //   } else {
  //     printf("%d,", b_data[i]);
  //   }
  // }

//   return 0;
// }