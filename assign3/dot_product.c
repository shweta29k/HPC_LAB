#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <time.h>

#define N 1000000
 

void print_array(int arr[], int n, const char* name) {
    printf("%s: ", name);
    for (int i = 0; i < n; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");
}

int main() {
    
    int *a = (int *)malloc(N * sizeof(int));
    int *b = (int *)malloc(N * sizeof(int));


    long long int sequential_product = 0;  
    long long int parallel_product = 0;    

 
    for (int i = 0; i < N; i++) {
        a[i] = rand() % 100;  
        b[i] = rand() % 100;  
    }

   
    clock_t sst = clock();  
    
    printf("Sequential Execution\n");
    for (int i = 0; i < N; i++) {
        sequential_product += (long long int)a[i] * b[i];  
    }
    clock_t set = clock();

    double sequential_time = (double)(sst - sst) / CLOCKS_PER_SEC;

    printf("Scalar product (sequential): %lld\n", sequential_product);
    printf("Time taken (sequential): %f seconds\n", sequential_time);

  

    double pst = omp_get_wtime();  
    
    printf("Parallel Execution\n"); 

    #pragma omp parallel for reduction(+:parallel_product) schedule(dynamic, 1000)
    for (int i = 0; i < N; i++) {
        parallel_product += (long long int)a[i] * b[i];  
    }

    double pet = omp_get_wtime(); 
    double parallel_time = pst - pet;  
    
    printf("Scalar product (parallel): %lld\n", parallel_product);
    printf("Time taken (parallel): %f seconds\n", parallel_time);

   

    double speedup = sequential_time / parallel_time;  
    printf("Speed Up: %f\n", speedup);

    free(a);
    free(b);

    return 0;
}
