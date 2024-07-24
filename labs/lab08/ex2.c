#include "ex2.h"

double dotp_naive(double* x, double* y, int arr_size) {
    double global_sum = 0.0;
    for (int i = 0; i < arr_size; i++)
        global_sum += x[i] * y[i];
    return global_sum;
}

// Critical Keyword
double dotp_critical(double* x, double* y, int arr_size) {
    double global_sum = 0.0;
    // Use the critical keyword here!'
    #pragma omp parallel for schedule(static)
    for (int i = 0; i < arr_size; i++) {
        #pragma omp critical
        global_sum += x[i] * y[i];
    }
    return global_sum;
    
}

// Reduction Keyword
double dotp_reduction(double* x, double* y, int arr_size) {
    double global_sum = 0.0;
    // TODO: Implement this function
    // Use the reduction keyword here!
    #pragma omp parallel for reduction(+:global_sum)
    for (int i = 0; i < arr_size; i++) {
        global_sum += x[i] * y[i];
    }
    return global_sum;
}

// Manual Reduction
double dotp_manual_reduction(double* x, double* y, int arr_size) {
    double global_sum = 0.0;
    #pragma omp parallel
    {
        int toatl = omp_get_num_threads();
        int cur = omp_get_thread_num();
        int chunk_size = arr_size / toatl;
        int res = arr_size % toatl;
        if (res == 0)
        {
            res = -1;
        } else if (res > 0 && cur >= res) {
            res--;
        } else {
            res = cur;
        }
        toatl--;
        double private_sum = 0.0;
        #pragma omp for
        for (int i = 0; i < arr_size; i++) {
            private_sum += x[i] * y[i];
            if ( (cur < toatl && i == (cur+1) * chunk_size + res) || i == arr_size - 1)
            {
                global_sum += private_sum;
            }
        }

        
    }
    return global_sum;
}
