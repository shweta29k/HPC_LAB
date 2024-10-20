#include <stdio.h>
#include <omp.h>

int main() {
    int num;
    double start_time, end_time;

    printf("Enter number of threads: ");
    scanf("%d", &num);

    omp_set_num_threads(num);

 
    

	#pragma omp parallel
    {
        printf("Hello, World from thread %d\n", omp_get_thread_num());
    }

    end_time = omp_get_wtime();
    printf("Time taken for Parallel Execution: %f seconds\n", end_time - start_time);


    return 0;
}

