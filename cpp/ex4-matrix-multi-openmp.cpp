#include <iostream>
#include <cstdlib>
#include <vector>
#include <omp.h>
#define VALIDATE true
#if VALIDATE
#include "validate.h"
#endif

void mat_mult(const size_t, const std::vector<int>&,
              const std::vector<int>&, std::vector<int>&);
void usage(char **);

int main(int argc, char **argv)
{
    std::vector<int> A, B, C;
    size_t i, n;
    double t0, t1;

    if (argc == 2)
        sscanf(argv[1], "%zu", &n);
    else {
        usage(argv);
        return 1;
    }

    A.resize(n * n);
    B.resize(n * n);
    C.resize(n * n);
    for (i = 0; i < n * n; ++i)
        A[i] = B[i] = i % 100;

    t0 = omp_get_wtime();
    mat_mult(n, A, B, C);
    t1 = omp_get_wtime();

#if VALIDATE
    if (!validate_mat_mult(n, A, B, C)) {
        std::cout << "Validation failed.\n";
        return 1;
    }
#endif

    std::cout << "Total time taken: " << t1 - t0 << ".\n";
    return 0;
}

void mat_mult(const size_t n, const std::vector<int>& A,
              const std::vector<int>& B, std::vector<int>& C)
{
    size_t i, j, k;
    #pragma omp parallel for default(none) shared(n,A,B,C) private(i,j,k)
    for (i = 0; i < n; ++i)
        for (k = 0; k < n; ++k)
            for (j = 0; j < n; ++j)
                C[i * n + j] += A[i * n + k] * B[k * n + j];
}

void usage(char **argv)
{
    std::cout << "Usage: " << argv[0] << " <length>\n";
}
