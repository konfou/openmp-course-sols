#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#define VALIDATE 0
#define SCHEDULE schedule(static)

int dot_prod(const size_t, const int * restrict, const int * restrict);
int validate_dp(const size_t, const int * restrict, const int * restrict, const int);
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
    for(i=0; i<n; ++i) u[i]=v[i]=i;

    t0 = omp_get_wtime();
    uv = dot_prod(n,u,v);
    t1 = omp_get_wtime();

#if VALIDATE
    if(!validate_dp(n,u,v,uv)) {
        printf("Validation failed.\n");
        return 1;
    }
#endif

    printf("Total time taken: %f.\n",t1-t0);

    free(u);
    free(v);
    return 0;
}

int dot_prod(const size_t n, const int * restrict u, const int * restrict v)
{
    int sum=0;
    size_t i;
    int *sums;
    sums = (int*)calloc(n,sizeof(int));
    #pragma omp parallel default(none) shared(sums,u,v) private(i)
    {
        int j = omp_get_thread_num();
        #pragma omp for SCHEDULE
        for(i=0; i<n; ++i) sums[j] += u[i]+v[i];
    }
    for(i=0; i<n; ++i) sum += sums[i];
    free(sums);
    return sum;
}

int validate_dp(const size_t n, const int * restrict u, const int * restrict v, const int uv)
{
    int sum=0;
    for(size_t i=0; i<n; ++i) sum += u[i]+v[i];
    return (uv!=sum) ? 0 : 1;
}

void usage(char **argv)
{
    printf("Usage: %s <length>\n",argv[0]);
}
