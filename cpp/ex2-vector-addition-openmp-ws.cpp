#include <iostream>
#include <cstdlib>
#include <vector>
#include <omp.h>
#define VALIDATE false
#if VALIDATE
    #include "validate.h"
#endif

void vec_add(const size_t, const std::vector<int>&, const std::vector<int>&, std::vector<int>&);
void usage(char**);

int main(int argc, char **argv)
{
    std::vector<int> u,v,w;
    size_t i,n;
    double t0,t1;

    if(argc==2)
        sscanf(argv[1],"%zu",&n);
    else {
        usage(argv);
        return 1;
    }

    u.reserve(n);
    v.reserve(n);
    w.reserve(n);
    for(i=0; i<n; ++i)
        u[i]=v[i]=i;

    t0 = omp_get_wtime();
    vec_add(n,u,v,w);
    t1 = omp_get_wtime();

#if VALIDATE
    if(!validate_vec_add(n,u,v,w)) {
        std::cout << "Validation failed.\n";
        return 1;
    }
#endif

    std::cout << "Total time taken: " << t1-t0 << ".\n";
    return 0;
}

void vec_add(const size_t n, const std::vector<int>& u, const std::vector<int>& v, std::vector<int>& w)
{
    size_t i;
    #pragma omp parallel for schedule(runtime) \
                    default(none) shared(u,v,w) private(i)
    for(i=0; i<n; ++i)
        w[i]=u[i]+v[i];
}

void usage(char **argv)
{
    std::cout << "Usage: " << argv[0] << " <length>\n";
}
