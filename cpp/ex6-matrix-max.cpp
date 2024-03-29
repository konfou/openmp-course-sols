#include <iostream>
#include <cstdlib>
#include <vector>
#include <omp.h>
#define VALIDATE true
#if VALIDATE
#include "validate.h"
#endif

int max(const size_t, const size_t, const std::vector<int>&);
void usage(char **);

int main(int argc, char **argv)
{
    std::vector<int> A;
    int max_val;
    size_t i, j, n;
    double t0, t1;

    if (argc == 2)
        sscanf(argv[1], "%zu", &n);
    else {
        usage(argv);
        return 1;
    }

    srand(42); // The Answer

    A.resize(n * n);
    for (i = 0; i < n; ++i)
        for (j = 0; j < n; ++j)
            A[i * n + j] = rand();

    t0 = omp_get_wtime();
    max_val = max(n, n, A);
    t1 = omp_get_wtime();

#if VALIDATE
    if (!validate_max(n, n, A, max_val)) {
        std::cout << "Validation failed.\n";
        return 1;
    }
#endif

    std::cout << "max(A) = " << max_val << std::endl;
    std::cout << "Total time taken: " << t1 - t0 << ".\n";
    return 0;
}

int max(const size_t n, const size_t m, const std::vector<int>& A)
{
    int max_val = A[0];
    size_t i, j;
    for (i = 0; i < n; ++i)
        for (j = 0; j < m; ++j)
            if (A[i * n + j] > max_val)
                max_val = A[i * n + j];
    return max_val;
}

void usage(char **argv)
{
    std::cout << "Usage: " << argv[0] << " <length>\n";
}
