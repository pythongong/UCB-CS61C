
#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

int main(int argc, char const *argv[])
{
    omp_set_num_threads(7);
    int limit = 19;
    
     #pragma omp parallel
    {
        int toatl = omp_get_num_threads();
        int cur = omp_get_thread_num();
        int chunk_size = limit / toatl;
        int res = limit % toatl;
        if (res == 0)
        {
            res = -1;
        } else if (res > 0 && cur >= res) {
            res--;
        } else {
            res = cur;
        }
        
        toatl--;
        #pragma omp for
        for(unsigned int i = 0; i < limit; i++) {
            
            if ( (cur < toatl && i == (cur+1) * chunk_size + res) || i == limit - 1)
            {
                printf("thread:%d, %d\n", omp_get_thread_num(), i);
                
            }
        }
        
    }
    return 0;
}
