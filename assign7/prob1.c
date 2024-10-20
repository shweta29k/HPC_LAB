#include <mpi.h>
#include <stdio.h>

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int world_size;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    int world_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

    if (world_size != 10) {
        if (world_rank == 0) {
            printf("Error: This program requires 10 processes, but %d were provided.\n", world_size);
        }
    } else {
        printf("Hello World from process %d out of %d!\n", world_rank, world_size);
    }

    MPI_Finalize();
    return 0;
}
