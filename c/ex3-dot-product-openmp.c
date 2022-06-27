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
    int sum=0;
    size_t i;
    #pragma omp parallel for \
                    default(none) shared(sum,n,u,v) private(i)
    for(i=0; i<n; ++i)
    #pragma omp critical
    {
        sum += u[i]+v[i];
    }
    return sum;
}

void usage(char **argv)
{
    printf("Usage: %s <length>\n",argv[0]);
}
