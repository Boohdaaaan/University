#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#define SIZE 1200

void fill_matrix(int *mat, int size) {
    for (int i = 0; i < size * size; i++) {
        mat[i] = rand() % 10;
    }
}


void multiply_matrix(int *a, int *b, int *c, int size) {
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            int sum = 0;
            for (int k = 0; k < size; k++) {
                sum += a[i * size + k] * b[k * size + j];
            }
            c[i * size + j] = sum;
        }
    }
}

int recursive_sum(int n) {
    if (n <= 0) return 0;
    return n + recursive_sum(n - 1);
}

int main() {
    srand(time(NULL));

    int *A = (int*)malloc(SIZE * SIZE * sizeof(int));
    int *B = (int*)malloc(SIZE * SIZE * sizeof(int));
    int *C = (int*)malloc(SIZE * SIZE * sizeof(int));

    fill_matrix(A, SIZE);
    fill_matrix(B, SIZE);

    clock_t start = clock();

    for (int iter = 0; iter < 5; iter++) {
        multiply_matrix(A, B, C, SIZE);
    }

    int rec = recursive_sum(1000);  // невелике навантаження рекурсією
    clock_t end = clock();

    double elapsed = (double)(end - start) / CLOCKS_PER_SEC;
    printf("Час виконання: %.3f секунд\n", elapsed);

    free(A);
    free(B);
    free(C);

    // Затримка для профілювання
    sleep(1);

    return 0;
}
