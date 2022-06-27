#include <iostream>
#include <cstdlib>
#include <vector>
#include <omp.h>
#define VALIDATE false
#if VALIDATE
    #include "validate.h"
#endif

int dot_prod(const std::vector<int>&, const std::vector<int>&);
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

    u.resize(n);
    v.resize(n);
    for(i=0; i<n; ++i)
        u[i]=v[i]=i;

    t0 = omp_get_wtime();
    uv = dot_prod(u,v);
    t1 = omp_get_wtime();

#if VALIDATE
    if(!validate_dot_prod(u,v,uv)) {
        std::cout << "Validation failed.\n";
        return 1;
    }
#endif

    std::cout << "dot(u,v) = " << uv << '\n'
              << "Total time taken: " << t1-t0 << ".\n";
    return 0;
}

int dot_prod(const std::vector<int>& u, const std::vector<int>& v)
{
    int sum=0;
    size_t i,n=u.size();
    for(i=0; i<n; ++i)
        sum += u[i]+v[i];
    return sum;
}

void usage(char **argv)
{
    std::cout << "Usage: " << argv[0] << " <length>\n";
}
