#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define VECTOR_SIZE 200

void scalar_add_static(double *vec, double scalar, int size, int chunk_size) {
    #pragma omp parallel for schedule(static, chunk_size)
    for (int i = 0; i < size; i++) {
        vec[i] += scalar;
    }
}

void scalar_add_dynamic(double *vec, double scalar, int size, int chunk_size) {
    #pragma omp parallel for schedule(dynamic, chunk_size)
    for (int i = 0; i < size; i++) {
        vec[i] += scalar;
    }
}

void scalar_add_with_nowait(double *vec, double scalar, int size) {
    #pragma omp parallel
    {
        #pragma omp for nowait
        for (int i = 0; i < size / 2; i++) {
            vec[i] += scalar;
        }

        #pragma omp for nowait
        for (int i = size / 2; i < size; i++) {
            vec[i] += scalar;
        }
    }
}

int main() {
    double vec[VECTOR_SIZE];
    double scalar = 5.0;
    int chunk_sizes[] = {1, 10, 20, 50, 100};  
    int num_chunk_sizes = sizeof(chunk_sizes) / sizeof(chunk_sizes[0]);

 
    srand(time(NULL));
    for (int i = 0; i < VECTOR_SIZE; i++) {
        vec[i] = rand() % 100;
    }

    printf("Static Scheduling Results:\n");
    for (int c = 0; c < num_chunk_sizes; c++) {
        int chunk_size = chunk_sizes[c];

        
        double vec_copy[VECTOR_SIZE];
        for (int i = 0; i < VECTOR_SIZE; i++) {
            vec_copy[i] = vec[i];
        }

        double start_time = omp_get_wtime();
        scalar_add_static(vec_copy, scalar, VECTOR_SIZE, chunk_size);
        double end_time = omp_get_wtime();

        printf("  Chunk Size %d: Time = %f seconds\n", chunk_size, end_time - start_time);
    }

 
    printf("\nDynamic Scheduling Results:\n");
    for (int c = 0; c < num_chunk_sizes; c++) {
        int chunk_size = chunk_sizes[c];

        
        double vec_copy[VECTOR_SIZE];
        for (int i = 0; i < VECTOR_SIZE; i++) {
            vec_copy[i] = vec[i];
        }

        double start_time = omp_get_wtime();
        scalar_add_dynamic(vec_copy, scalar, VECTOR_SIZE, chunk_size);
        double end_time = omp_get_wtime();

        printf("  Chunk Size %d: Time = %f seconds\n", chunk_size, end_time - start_time);
    }

    
    printf("\nNowait Clause Results:\n");

   
    double vec_copy[VECTOR_SIZE];
    for (int i = 0; i < VECTOR_SIZE; i++) {
        vec_copy[i] = vec[i];
    }

    double start_time = omp_get_wtime();
    scalar_add_with_nowait(vec_copy, scalar, VECTOR_SIZE);
    double end_time = omp_get_wtime();

    printf("  With nowait: Time = %f seconds\n", end_time - start_time);

    return 0;
}
