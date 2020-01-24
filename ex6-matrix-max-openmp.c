#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#define VALIDATE 0

int max(const size_t n, const int * restrict);
int validate_mm(const size_t n, const int * restrict, const int);
void usage(char**);

int main(int argc, char **argv)
{
    int *A,max_val;
    size_t i,j,n;
    double t0,t1;

    if(argc==2)
        sscanf(argv[1],"%zu",&n);
    else {
        usage(argv);
        return 1;
    }

    A = (int*)malloc(n*n*sizeof(int));
    for(i=0; i<n; ++i)
        for(j=0; j<n; ++j) A[i*n+j]=i*n+j;

    t0 = omp_get_wtime();
    max_val = max(n,A);
    t1 = omp_get_wtime();

#if VALIDATE
    if(!validate_mm(n,A,max_val)) {
        printf("Validation failed.\n");
        return 1;
    }
#endif

    printf("Total time taken: %f.\n",t1-t0);

    free(A);
    return 0;
}

int max(const size_t n, const int * restrict A)
{
    size_t i,j;
    int max_val=A[0];
    #pragma omp parallel for default(none) shared(max_val,A) private(i,j)
    for(i=0; i<n; ++i)
        for(j=0; j<n; ++j)
            if(A[i*n+j]>max_val) {
                #pragma omp critical
                {
                    max_val=A[i*n+j];
                }
            }
    return max_val;
}

int validate_mm(const size_t n, const int * restrict A, const int max_val)
{
    size_t i,j;
    int tmax=A[0];
    for(i=0; i<n; ++i)
        for(j=0; j<n; ++j)
            if(A[i*n+j]>tmax) tmax=A[i*n+j];
    return (max_val!=tmax) ? 0 : 1;
}

void usage(char **argv)
{
    printf("Usage: %s <length>\n",argv[0]);
}
