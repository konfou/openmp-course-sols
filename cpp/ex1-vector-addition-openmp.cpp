#include <iostream>
#include <cstdlib>
#include <vector>
#include <omp.h>
#define VALIDATE false
#if VALIDATE
    #include "validate.h"
#endif

void vec_add(const std::vector<int>&, const std::vector<int>&, std::vector<int>&);
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

    u.resize(n);
    v.resize(n);
    w.resize(n);
    for(i=0; i<n; ++i)
        u[i]=v[i]=i;

    t0 = omp_get_wtime();
    vec_add(u,v,w);
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

void vec_add(const std::vector<int>& u, const std::vector<int>& v, std::vector<int>& w)
{
    int id,nt;
    size_t i,is,ie,n=w.size();;
    #pragma omp parallel default(none) shared(n,u,v,w) private(i,is,ie,id,nt)
    {
        id = omp_get_thread_num();
        nt = omp_get_num_threads();
        is = id*n/nt;
        ie = (id==nt-1) ? n : (id+1)*n/nt;
        for(i=is; i<ie; ++i)
            w[i]=u[i]+v[i];
    }
}

void usage(char **argv)
{
    std::cout << "Usage: " << argv[0] << " <length>\n";
}
