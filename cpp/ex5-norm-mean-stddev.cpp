#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <vector>
#include <omp.h>
#define VALIDATE false
#if VALIDATE
    #include "validate.h"
#endif

std::vector<double> normrnd(const size_t, const size_t, const double, const double);
double mean(const size_t, const size_t, const std::vector<double>&);
double stddev(const size_t, const size_t, const double, const std::vector<double>&);
void usage(char**);

int main(int argc, char **argv)
{
    std::vector<double> A;
    double avgA,stdA;
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
    avgA = mean(n,n,A);
    stdA = stddev(n,n,avgA,A);
    t1 = omp_get_wtime();

#if VALIDATE
    if(!validate_mean(n,n,A,avgA)) {
        std::cout << "Mean value validation failed.\n";
        return 1;
    }
    if(!validate_std(n,n,avg,A,stdA)) {
        std::cout << "Standard deviation validation failed.\n";
        return 1;
    }
#endif

    std::cout << "mean(A) = " << avgA << std::endl
              << "std(A) = " << stdA << std::endl;
    std::cout << "Total time taken: " << t1-t0 << ".\n";
    return 0;
}

std::vector<double> normrnd(const size_t n, const size_t m, const double mu, const double sigma)
{
    std::vector<double> A(n*m);
    double u1,u2,z0;
    for(size_t i=0; i<n*m; ++i) {
        u1 = (double)rand()/(double)RAND_MAX;
        u2 = (double)rand()/(double)RAND_MAX;
        z0 = sqrt(-2*log(u1))*cos(6.28*u2);     /* Box–Muller transform */
        A[i] = mu+sigma*z0;
    }
    return A;
}

double mean(const size_t n, const size_t m, const std::vector<double>& A)
{
    double sum=0;
    size_t i;
    for(i=0; i<n*m; ++i)
        sum += A[i];
    return sum/(n*m);
}

double stddev(const size_t n, const size_t m, const double avg, const std::vector<double>& A)
{
    double sum=0;
    size_t i;
    for(i=0; i<n*m; ++i)
        sum += (A[i]-avg)*(A[i]-avg);
    return sqrt(sum/(n*m-1));
}

void usage(char **argv)
{
    std::cout << "Usage: " << argv[0] << " <length>\n";
}
