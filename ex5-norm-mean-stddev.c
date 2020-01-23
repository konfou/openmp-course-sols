#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <omp.h>
#define EPS 0.000001

double *normrnd(const size_t, const size_t, const double, const double);
double mean(const size_t, const size_t, const double * restrict);
double std(const size_t, const size_t, const double, const double * restrict);
int validate_mm(const size_t, const size_t, const double * restrict, const double);
int validate_ms(const size_t, const size_t, const double, const double * restrict, const double);
void usage(char**);

int main(int argc, char **argv)
{
    double *A,avg,stddev;
    size_t n;
    double t0,t1;

    srand(time(0));

    if(argc==2)
        sscanf(argv[1],"%zu",&n);
    else {
        usage(argv);
        return 1;
    }

    A = normrnd(n,n,0,1); /* standard normal distribution: (mu,sigma)=(0,1) */

    t0 = omp_get_wtime();
    avg = mean(n,n,A);
    stddev = std(n,n,avg,A);
    t1 = omp_get_wtime();
    if(!validate_mm(n,n,A,avg)) {
        printf("Mean value validation failed.\n");
        return 1;
    }
    if(!validate_ms(n,n,avg,A,stddev)) {
        printf("Standard deviation validation failed.\n");
        return 1;
    }
    printf("Total time taken: %f.\n",t1-t0);

    free(A);
    return 0;
}

double *normrnd(const size_t n, const size_t m, const double mu, const double sigma)
{
    size_t i,j;
    double *A = (double*)malloc(n*m*sizeof(double));
    double u1,u2,z0;

    for(i=0;i<n;++i)
        for(j=0;j<m;++j) {
            u1 = (double)rand()/(double)RAND_MAX;
            u2 = (double)rand()/(double)RAND_MAX;
            z0 = sqrt(-2*log(u1))*cos(6.28*u2);     /* Box–Muller transform */
            A[i*n+j] = mu+sigma*z0;
        }

    return A;
}

double mean(const size_t n, const size_t m, const double * restrict A)
{
    size_t i;
    double sum=0;
    for(i=0; i<n*m; ++i) sum += A[i];
    return sum/(n*m);
}

double std(const size_t n, const size_t m, const double avg, const double * restrict A)
{
    size_t i;
    double sum=0;
    for(i=0; i<n*m; ++i) sum += (A[i]-avg)*(A[i]-avg);
    return sqrt(sum/(n-1));
}

int validate_mm(const size_t n, const size_t m, const double * restrict A, const double a)
{
    size_t i;
    double sum=0;
    for(i=0; i<n*m; ++i) sum += A[i];
    return (fabs(a-sum/(n*m))>EPS) ? 0 : 1;
}

int validate_ms(const size_t n, const size_t m, const double mean, const double * restrict A, const double s)
{
    size_t i;
    double sum=0;
    for(i=0; i<n*m; ++i) sum += (A[i]-mean)*(A[i]-mean);
    return (fabs(s-sqrt(sum/(n-1)))>EPS) ? 0 : 1;
}

void usage(char **argv)
{
    printf("Usage: %s <length>\n",argv[0]);
}
