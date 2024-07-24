#include "ex1.h"


void v_add_naive(double* x, double* y, double* z) {
    #pragma omp parallel
    {
        for(int i=0; i<ARRAY_SIZE; i++)
            z[i] = x[i] + y[i];
    }
}

// Adjacent Method
void v_add_optimized_adjacent(double* x, double* y, double* z) {
    #pragma omp parallel
    {
        int toatl = omp_get_num_threads();
        int cur = omp_get_thread_num();
        for(int i=cur; i<ARRAY_SIZE; i+=toatl) {
            z[i] = x[i] + y[i];
        }
    }
}

// Chunks Method
void v_add_optimized_chunks(double* x, double* y, double* z) {
    #pragma omp parallel
    {
        int toatl = omp_get_num_threads();
        int cur = omp_get_thread_num();
        int chunk_size = ARRAY_SIZE / toatl;
        int i = chunk_size * cur;
        int limit = cur == toatl - 1 ? ARRAY_SIZE : chunk_size * (cur+1);
        for(; i<limit; i++) {
            z[i] = x[i] + y[i];
        }
    }
}

