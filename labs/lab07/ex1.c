#include <time.h>
#include <stdio.h>
#include <x86intrin.h>
#include "ex1.h"

long long int sum(int vals[NUM_ELEMS]) {
    clock_t start = clock();

    long long int sum = 0;
    for(unsigned int w = 0; w < OUTER_ITERATIONS; w++) {
        for(unsigned int i = 0; i < NUM_ELEMS; i++) {
            if(vals[i] >= 128) {
                sum += vals[i];
            }
        }
    }
    clock_t end = clock();
    printf("Time taken: %Lf s\n", (long double)(end - start) / CLOCKS_PER_SEC);
    return sum;
}

long long int sum_unrolled(int vals[NUM_ELEMS]) {
    clock_t start = clock();
    long long int sum = 0;

    for(unsigned int w = 0; w < OUTER_ITERATIONS; w++) {
        for(unsigned int i = 0; i < NUM_ELEMS / 4 * 4; i += 4) {
            if(vals[i] >= 128) sum += vals[i];
            if(vals[i + 1] >= 128) sum += vals[i + 1];
            if(vals[i + 2] >= 128) sum += vals[i + 2];
            if(vals[i + 3] >= 128) sum += vals[i + 3];
        }

        // TAIL CASE, for when NUM_ELEMS isn't a multiple of 4
        // NUM_ELEMS / 4 * 4 is the largest multiple of 4 less than NUM_ELEMS
        // Order is important, since (NUM_ELEMS / 4) effectively rounds down first
        for(unsigned int i = NUM_ELEMS / 4 * 4; i < NUM_ELEMS; i++) {
            if (vals[i] >= 128) {
                sum += vals[i];
            }
        }
    }
    clock_t end = clock();
    printf("Time taken: %Lf s\n", (long double)(end - start) / CLOCKS_PER_SEC);
    return sum;
}

long long int sum_simd(int vals[NUM_ELEMS]) {
    clock_t start = clock();
    __m128i _127 = _mm_set1_epi32(127); // This is a vector with 127s in it... Why might you need this?
    long long int result = 0; // This is where you should put your final result!
    /* DO NOT MODIFY ANYTHING ABOVE THIS LINE (in this function) */

    for(unsigned int w = 0; w < OUTER_ITERATIONS; w++) {
        /* YOUR CODE GOES HERE */
        __m128i sum_vector = _mm_setzero_si128();
        int tmp_arr[4];
        unsigned int limit = (NUM_ELEMS / 16) * 16;
        unsigned int i = 0;
        for(;  i < limit; i += 16) {
            // Load array elements into a temporary vector register
            __m128i tmp = _mm_loadu_si128((__m128i *) (vals+i));
            // if the array elements <= 127, make it 0
            __m128i comparaed_res = _mm_cmpgt_epi32(tmp, _127);
            __m128i and_res = _mm_and_si128(tmp, comparaed_res);
            // Add to existing sum vector
            sum_vector = _mm_add_epi32(sum_vector, and_res);

            // unroll 4 loops here
            tmp = _mm_loadu_si128((__m128i *) (vals+i+4));
            comparaed_res = _mm_cmpgt_epi32(tmp, _127);
            and_res = _mm_and_si128(tmp, comparaed_res);
            // Add to existing sum vector
            sum_vector = _mm_add_epi32(sum_vector, and_res);

            tmp = _mm_loadu_si128((__m128i *) (vals+i+8));
            comparaed_res = _mm_cmpgt_epi32(tmp, _127);
            and_res = _mm_and_si128(tmp, comparaed_res);
            // Add to existing sum vector
            sum_vector = _mm_add_epi32(sum_vector, and_res);

            tmp = _mm_loadu_si128((__m128i *) (vals+i+12));
            comparaed_res = _mm_cmpgt_epi32(tmp, _127);
            and_res = _mm_and_si128(tmp, comparaed_res);
            // Add to existing sum vector
            sum_vector = _mm_add_epi32(sum_vector, and_res);
        }

        _mm_storeu_si128((__m128i *) tmp_arr, sum_vector);
        // Collect values from sum_vec in a single integer
        result += tmp_arr[0] + tmp_arr[1] + tmp_arr[2] + tmp_arr[3];
        /* tail case. */
        for(; i < NUM_ELEMS; i++) {
            if (vals[i] >= 128) {
                result += vals[i];
            }
        }
    }

    /* DO NOT MODIFY ANYTHING BELOW THIS LINE (in this function) */
    clock_t end = clock();
    printf("Time taken: %Lf s\n", (long double)(end - start) / CLOCKS_PER_SEC);
    return result;
}

long long int sum_simd_unrolled(int vals[NUM_ELEMS]) {
    clock_t start = clock();
    __m128i _127 = _mm_set1_epi32(127);
    long long int result = 0;
    /* DO NOT MODIFY ANYTHING ABOVE THIS LINE (in this function) */

    for(unsigned int w = 0; w < OUTER_ITERATIONS; w++) {
        /* YOUR CODE GOES HERE */
        __m128i sum_vector = _mm_setzero_si128();
        int tmp_arr[4];
        unsigned int limit = (NUM_ELEMS / 4) * 4;
        unsigned int i = 0;
        for(;  i < limit; i += 4) {
            // Load array elements into a temporary vector register
            __m128i tmp = _mm_loadu_si128((__m128i *) (vals+i));
            // if the array elements <= 127, make it 0
            __m128i comparaed_res = _mm_cmpgt_epi32(tmp, _127);
            __m128i and_res = _mm_and_si128(tmp, comparaed_res);
            // Add to existing sum vector
            sum_vector = _mm_add_epi32(sum_vector, and_res);
        }

        _mm_storeu_si128((__m128i *) tmp_arr, sum_vector);
        // Collect values from sum_vec in a single integer
        result += tmp_arr[0] + tmp_arr[1] + tmp_arr[2] + tmp_arr[3];
    
        /* tail case. */
        for(; i < NUM_ELEMS; i++) {
            if (vals[i] >= 128) {
                result += vals[i];
            }
        }
    }

    /* DO NOT MODIFY ANYTHING BELOW THIS LINE (in this function) */
    clock_t end = clock();
    printf("Time taken: %Lf s\n", (long double)(end - start) / CLOCKS_PER_SEC);
    return result;
}
