#include "transpose.h"

/* The naive transpose function as a reference. */
void transpose_naive(int n, int blocksize, int *dst, int *src) {
    for (int x = 0; x < n; x++) {
        for (int y = 0; y < n; y++) {
            dst[y + x * n] = src[x + y * n];
        }
    }
}

/* Implement cache blocking below. You should NOT assume that n is a
 * multiple of the block size. */
void transpose_blocking(int n, int blocksize, int *dst, int *src) {
    // YOUR CODE HERE

    int z = blocksize;
    for (; z <= n; z += blocksize) {
        for (int x = 0; x < n; x++) {
            for (int y = z - blocksize; y < z; y++) {
                dst[y + x * n] = src[x + y * n];
            }
        }
    }
    
    z -= blocksize;
    if (z != n) {
        for (int x = 0; x < n; x++) {
            for (int y = z; y < n; y++) {
                dst[y + x * n] = src[x + y * n];
            }
        }
    }
}
