#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#define VALIDATE 1
#if VALIDATE
    #include "validate.h"
#endif

int dot_prod(const size_t, const int * restrict, const int * restrict);
void usage(char**);

int main(int argc, char **argv)
{
    int *u,*v,uv;
    size_t i,n;
    double t0,t1;

    if(argc==2)
        sscanf(argv[1],"%zu",&n);
    else {
        usage(argv);
        return 1;
    }

    u = (int*)malloc(n*sizeof(int));
    v = (int*)malloc(n*sizeof(int));
    for(i=0; i<n; ++i)
        u[i]=v[i]=i;

    t0 = omp_get_wtime();
    uv = dot_prod(n,u,v);
    t1 = omp_get_wtime();

#if VALIDATE
    if(!validate_dot_prod(n,u,v,uv)) {
        printf("Validation failed.\n");
        return 1;
    }
#endif

    printf("dot(u,v) = %d\n",uv);
    printf("Total time taken: %f.\n",t1-t0);

    free(u);
    free(v);
    return 0;
}

int dot_prod(const size_t n, const int * restrict u, const int * restrict v)
{
    int sum=0,nt=omp_get_max_threads();
    int *sums = (int*)calloc(nt,sizeof(int));
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
    free(sums);
    return sum;
}

void usage(char **argv)
{
    printf("Usage: %s <length>\n",argv[0]);
}
