#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

void vector_scalar_addition(int *vector, int val, int size) {
    #pragma omp parallel for
    for (int i = 0; i < size; i++) {
        vector[i] += val;
    }
}

int main() {
    int size = 10000000; 
    int val = 5;
    int *vector = (int *)malloc(size * sizeof(int));

    for (int i = 0; i < size; i++) {
        vector[i] = i * 1.0f;
    }

    double start_time = omp_get_wtime();
    vector_scalar_addition(vector, val, size);
    double end_time = omp_get_wtime();
	
    printf("Time taken for vector-scalar addition: %f seconds\n", end_time - start_time);
	
    free(vector);
    return 0;
}

