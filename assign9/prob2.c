#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void multiply_matrices(int *A, int *B, int *C, int N, int rows_per_process, int rank, int size) {
    for (int i = 0; i < rows_per_process; i++) {
        for (int j = 0; j < N; j++) {
            C[i * N + j] = 0;
            for (int k = 0; k < N; k++) {
                C[i * N + j] += A[i * N + k] * B[k * N + j];
            }
        }
    }
}

int main(int argc, char** argv) {
    int rank, size;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int N;
    if (rank == 0) {
        printf("Enter the size of the matrices (N x N): ");
        scanf("%d", &N);
    }

    MPI_Bcast(&N, 1, MPI_INT, 0, MPI_COMM_WORLD);

    int rows_per_process = N / size;
    int *A = NULL;
    int *B = NULL;
    int *C = (int*)malloc(rows_per_process * N * sizeof(int));
    int *local_A = (int*)malloc(rows_per_process * N * sizeof(int));
    int *local_B = NULL;

    if (rank == 0) {
        A = (int*)malloc(N * N * sizeof(int));
        B = (int*)malloc(N * N * sizeof(int));
        srand(time(0));
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                A[i * N + j] = rand() % 10;
                B[i * N + j] = rand() % 10;
            }
        }
    }

    local_B = (int*)malloc(N * N * sizeof(int));
    MPI_Bcast(B, N * N, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Scatter(A, rows_per_process * N, MPI_INT, local_A, rows_per_process * N, MPI_INT, 0, MPI_COMM_WORLD);

    double start_time = MPI_Wtime();
    multiply_matrices(local_A, local_B, C, N, rows_per_process, rank, size);
    if (rank == 0) {
        A = (int*)malloc(N * N * sizeof(int));
    }
    MPI_Gather(C, rows_per_process * N, MPI_INT, A, rows_per_process * N, MPI_INT, 0, MPI_COMM_WORLD);

    double end_time = MPI_Wtime();
    double execution_time = end_time - start_time;

    if (rank == 0) {
        printf("Matrix multiplication completed.\n");
        printf("Time taken with %d processes: %f seconds\n", size, execution_time);
        double T1 = execution_time;
        printf("Speedup for %d processes: %f\n", size, T1 / execution_time);
        free(A);
    }

    free(local_A);
    free(local_B);
    free(C);
    MPI_Finalize();
    return 0;
}

