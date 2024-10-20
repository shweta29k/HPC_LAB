#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

#define N 100 

int main(int argc, char** argv) {
    int rank, size, i, j;
    double A[N][N], x[N], y[N], local_y[N / 2]; // Local result for each process
    double start_time, end_time, local_time, max_time, serial_time = 1.0; // Add serial time value for comparison

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);


  
    if (rank == 0) {
        for (i = 0; i < N; i++) {
            x[i] = i + 1;  // Vector x = [1, 2, 3, 4]
            for (j = 0; j < N; j++) {
                A[i][j] = i + j + 1;  // Example matrix A
            }
        }
    }

  
    MPI_Bcast(x, N, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    // Scatter the rows of matrix A among the processes
    double local_A[N / 2][N];
    MPI_Scatter(A, N * N / 2, MPI_DOUBLE, local_A, N * N / 2, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    // Start timing the computation
    start_time = MPI_Wtime();

    // Perform the local computation (matrix-vector multiplication)
    for (i = 0; i < N / 2; i++) {
        local_y[i] = 0.0;
        for (j = 0; j < N; j++) {
            local_y[i] += local_A[i][j] * x[j];
        }
    }

    // Gather the local results to the root process
    MPI_Gather(local_y, N / 2, MPI_DOUBLE, y, N / 2, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    // Stop timing the computation
    end_time = MPI_Wtime();
    local_time = end_time - start_time;

    // Find the maximum time among all processes (for fairness in parallel execution)
    MPI_Reduce(&local_time, &max_time, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);

    // Root process prints the result and the timing information
    if (rank == 0) {
        printf("Resultant vector y = \n");
        for (i = 0; i < N; i++) {
            printf("%f\n", y[i]);
        }
        printf("Time taken with %d processes: %f seconds\n", size, max_time);

        // Assuming the serial time is pre-calculated or measured separately
        double speedup = serial_time / max_time;
        printf("Speedup with %d processes: %f\n", size, speedup);
    }

    MPI_Finalize();
    return 0;
}
