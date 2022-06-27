#include <iostream>
#include <cstdlib>
#include <cmath>
#include <vector>
#include <omp.h>
#define VALIDATE 1

/*  based on the unbalanced example found in
 *  https://www.astro.auth.gr/~niksterg/courses/progtools/imbalance.c
 */

std::vector<double> rand_vec(const size_t);
void set_iterations(const size_t, const int, const int, std::vector<double>&);
void iterate(const size_t, const std::vector<double>&, const std::vector<double>&, std::vector<double>&);
void usage(char**);

int main(int argc, char **argv)
{
    std::vector<double> b,s,a;
    size_t n;
    double t0,t1;

    if(argc==2)
        sscanf(argv[1],"%zu",&n);
    else {
        usage(argv);
        return 1;
    }

    srand(33); // Based on Pythagoras

    s.resize(n);
    a.resize(n);

    b = rand_vec(n);
    set_iterations(n,2,n,s);

    std::cout << "Working...\n";

    t0 = omp_get_wtime();
    iterate(n,b,s,a);
    t1 = omp_get_wtime();

    std::cout << "Total time taken: " << t1-t0 << ".\n";

#if VALIDATE
    double sum=0;
    for(size_t i=0; i<n; ++i)
        sum += a[i];
    std::cout << "Sum: " << sum << std::endl;
#endif

    return 0;
}

std::vector<double> rand_vec(const size_t n)
{
    std::vector<double> v(n);
    for(size_t i=0; i<n; ++i)
        v[i] = (double)rand()/(double)RAND_MAX;
    return v;
}

void set_iterations(const size_t n, const int lo, const int hi, std::vector<double>& s)
{
    for(size_t i=0; i<n; ++i)
        s[i] = (i<100 || !i%279) ? hi : lo;
}

void iterate(const size_t n, const std::vector<double>& b, const std::vector<double>& s, std::vector<double>& a)
{
    size_t i,j,k;
    omp_set_nested(0);  //protect against data races
    #pragma omp parallel for schedule(runtime) \
                    default(none) shared(n,b,s,a) private(i,j,k)
    for(i=0; i<n; ++i)
        for(j=1; j<s[i]; ++j)
            for(k=1; k<j; ++k)
                a[i] += log(j+k)*pow(b[i],4.0)/(n*n);
}

void usage(char **argv)
{
    std::cout << "Usage: " << argv[0] << " <length>\n";
}
