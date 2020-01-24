#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#define VALIDATE 0

void mat_prod(const size_t n, const int * restrict, const int * restrict, int * restrict);
int validate_mp(const size_t n, const int * restrict, const int * restrict, const int * restrict);
void usage(char**);

int main(int argc, char **argv)
{
    int *A,*B,*C;
    size_t i,n;
    double t0,t1;

    if(argc==2)
        sscanf(argv[1],"%zu",&n);
    else {
        usage(argv);
        return 1;
    }

    A = (int*)malloc(n*n*sizeof(int));
    B = (int*)malloc(n*n*sizeof(int));
    C = (int*)calloc(n*n,sizeof(int));
    for(i=0; i<n*n; ++i) A[i]=B[i]=i;

    t0 = omp_get_wtime();
    mat_prod(n,A,B,C);
    t1 = omp_get_wtime();

#if VALIDATE
    if(!validate_mp(n,A,B,C)) {
        printf("Validation failed.\n");
        return 1;
    }
#endif

    printf("Total time taken: %f.\n",t1-t0);

    free(A);
    free(B);
    free(C);
    return 0;
}

void mat_prod(const size_t n, const int * restrict A, const int * restrict B, int * restrict C)
{
    size_t i,j,k;
    for(i=0; i<n; ++i)
        for(k=0; k<n; ++k)
            for(j=0; j<n; ++j)
                C[i*n+j]+=A[i*n+k]*B[k*n+j];
}

int validate_mp(const size_t n, const int * restrict A, const int * restrict B, const int * restrict C)
{
    int *D = (int*)calloc(n*n,sizeof(int));
    size_t i,j,k;
    for(i=0; i<n; ++i)
        for(k=0; k<n; ++k)
            for(j=0; j<n; ++j)
                D[i*n+j]+=A[i*n+k]*B[k*n+j];
    for(i=0; i<n*n; ++i)
        if(D[i]!=C[i]) {
            free(D);
            return 0;
        }
    free(D);
    return 1;
}

void usage(char **argv)
{
    printf("Usage: %s <length>\n",argv[0]);
}
