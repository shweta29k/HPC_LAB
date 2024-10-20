#include <stdio.h>
#include <omp.h>

void matrixVectorMultiply(int N, int A[N][N], int B[N], int C[N]) {
    int i, j;

    #pragma omp parallel for private(i, j) shared(A, B, C)
    for (i = 0; i < N; i++) {
        C[i] = 0;
        for (j = 0; j < N; j++) {
            C[i] += A[i][j] * B[j];
        }
    }
}

int main() {
    int N;
    printf("Enter the size of the matrix (NxN): ");
    scanf("%d", &N);

    int A[N][N], B[N], C[N];

    printf("Enter elements of matrix A:\n");
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            scanf("%d", &A[i][j]);
        }
    }

    printf("Enter elements of vector B:\n");
    for (int i = 0; i < N; i++) {
        scanf("%d", &B[i]);
    }

    matrixVectorMultiply(N, A, B, C);

    printf("Resultant Vector C (Matrix-Vector Multiplication):\n");
    for (int i = 0; i < N; i++) {
        printf("%d ", C[i]);
    }
    printf("\n");

    return 0;
}

