#include <iostream>
#include <cstdlib>
#include <vector>
#include <omp.h>
#define VALIDATE false
#if VALIDATE
    #include "validate.h"
#endif

int dot_prod(const size_t, const std::vector<int>&, const std::vector<int>&);
void usage(char**);

int main(int argc, char **argv)
{
    std::vector<int> u,v;
    int uv;
    size_t i,n;
    double t0,t1;

    if(argc==2)
        sscanf(argv[1],"%zu",&n);
    else {
        usage(argv);
        return 1;
    }

    v.reserve(n);
    u.reserve(n);
    for(i=0; i<n; ++i)
        u[i]=v[i]=i;

    t0 = omp_get_wtime();
    uv = dot_prod(n,u,v);
    t1 = omp_get_wtime();

#if VALIDATE
    if(!validate_dot_prod(n,u,v,uv)) {
        std::cout << "Validation failed.\n";
        return 1;
    }
#endif

    std::cout << "dot(u,v) = " << uv << '\n'
              << "Total time taken: " << t1-t0 << ".\n";
    return 0;
}

int dot_prod(const size_t n, const std::vector<int>& u, const std::vector<int>& v)
{
    int sum=0,nt=omp_get_max_threads();
    std::vector<int> sums(nt);
    size_t i;
    #pragma omp parallel default(none) shared(sums,n,u,v) private(i)
    {
        int j = omp_get_thread_num();
        #pragma omp for
        for(i=0; i<n; ++i)
            sums[j] += u[i]+v[i];
    }
    for(i=0; i<(size_t)nt; ++i)
        sum += sums[i];
    return sum;
}

void usage(char **argv)
{
    std::cout << "Usage: " << argv[0] << " <length>\n";
}
