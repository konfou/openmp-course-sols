#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#define VALIDATE 1
#if VALIDATE
#include "validate.h"
#endif

int max(const size_t, const size_t, const int *restrict);
void usage(char **);

int main(int argc, char **argv)
{
    int *A, max_val;
    size_t i, j, n;
    double t0, t1;

    if (argc == 2)
        sscanf(argv[1], "%zu", &n);
    else {
        usage(argv);
        return 1;
    }

    srand(42); // The Answer

    A = (int *) malloc(n * n * sizeof(int));
    for (i = 0; i < n; ++i)
        for (j = 0; j < n; ++j)
            A[i * n + j] = rand();

    t0 = omp_get_wtime();
    max_val = max(n, n, A);
    t1 = omp_get_wtime();

#if VALIDATE
    if (!validate_max(n, n, A, max_val)) {
        printf("Validation failed.\n");
        return 1;
    }
#endif

    printf("max(A) = %d\n", max_val);
    printf("Total time taken: %f.\n", t1 - t0);

    free(A);
    return 0;
}

int max(const size_t n, const size_t m, const int *restrict A)
{
    int max_val = A[0];
    size_t i, j;
    #pragma omp parallel for default(none) shared(max_val,n,m,A) private(i,j)
    for (i = 0; i < n; ++i)
        for (j = 0; j < m; ++j)
            #pragma omp critical
            if (A[i * n + j] > max_val)
                max_val = A[i * n + j];
    return max_val;
}

void usage(char **argv)
{
    printf("Usage: %s <length>\n", argv[0]);
}
