#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

void vector_add(const size_t, const int * restrict, const int * restrict, int * restrict);
int validate_va(const size_t, const int * restrict, const int * restrict, const int * restrict);
void usage(char**);

int main(int argc, char **argv)
{
    int *u,*v,*w;
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
    w = (int*)malloc(n*sizeof(int));
    for(i=0; i<n; ++i) u[i]=v[i]=i;

    t0 = omp_get_wtime();
    vector_add(n,u,v,w);
    t1 = omp_get_wtime();
    if(!validate_va(n,u,v,w)) {
        printf("Validation failed.\n");
        return 1;
    }
    printf("Total time taken: %f.\n",t1-t0);

    free(u);
    free(v);
    free(w);
    return 0;
}

void vector_add(const size_t n, const int * restrict u, const int * restrict v, int * restrict w)
{
    size_t i,is,ie;
    int id,nt;
    #pragma omp parallel default(none) shared(u,v,w) private(i,is,ie,id,nt)
    {
        id = omp_get_thread_num();
        nt = omp_get_num_threads();
        is = id*n/nt;
        ie = (id==nt-1) ? n : (id+1)*n/nt;
        for(i=is; i<ie; ++i) w[i]=u[i]+v[i];
    }
}

int validate_va(const size_t n, const int * restrict u, const int * restrict v, const int * restrict w)
{
    for(size_t i=0; i<n; ++i)
        if(w[i]!=u[i]+v[i]) return 0;
    return 1;
}

void usage(char **argv)
{
    printf("Usage: %s <length>\n",argv[0]);
}