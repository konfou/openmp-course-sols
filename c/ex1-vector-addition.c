#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#define VALIDATE 1
#if VALIDATE
#include "validate.h"
#endif

void vec_add(const size_t, const int *restrict, const int *restrict,
             int *restrict);
void usage(char **);

int main(int argc, char **argv)
{
    int *u, *v, *w;
    size_t i, n;
    double t0, t1;

    if (argc == 2)
        sscanf(argv[1], "%zu", &n);
    else {
        usage(argv);
        return 1;
    }

    u = (int *) malloc(n * sizeof(int));
    v = (int *) malloc(n * sizeof(int));
    w = (int *) malloc(n * sizeof(int));
    for (i = 0; i < n; ++i)
        u[i] = v[i] = i;

    t0 = omp_get_wtime();
    vec_add(n, u, v, w);
    t1 = omp_get_wtime();

#if VALIDATE
    if (!validate_vec_add(n, u, v, w)) {
        printf("Validation failed.\n");
        return 1;
    }
#endif

    printf("Total time taken: %f.\n", t1 - t0);

    free(u);
    free(v);
    free(w);
    return 0;
}

void vec_add(const size_t n, const int *restrict u, const int *restrict v,
             int *restrict w)
{
    size_t i;
    for (i = 0; i < n; ++i)
        w[i] = u[i] + v[i];
}

void usage(char **argv)
{
    printf("Usage: %s <length>\n", argv[0]);
}
