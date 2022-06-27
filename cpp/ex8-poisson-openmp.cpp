#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cmath>
#include <vector>
#include <omp.h>

struct Opts
{
   double a,b,ua,ub,dx,its,tol;
   size_t n;
};

void init_rho(std::vector<double>&, const struct Opts);
void jacobi(std::vector<double>&, const std::vector<double>&, const struct Opts);
double residual(const std::vector<double>&, const std::vector<double>&, const struct Opts);
void output(const std::vector<double>&, const std::vector<double>&, const size_t);

static inline double rhox(const double x)
{
    return sin(1.57*x);
}

int main(void)
{
    struct Opts o;
    std::vector<double> u,rho;
    double t0,t1;

    o.a = 0.0;
    o.b = 1.0;
    o.ua = 0.0;
    o.ub = 0.0;
    o.dx = 0.00001;
    o.its = 200000;
    o.tol = 0.00001;

    o.n = (int)(fabs(o.b-o.a)/o.dx);
    u.resize(o.n);
    rho.reserve(o.n);

    u[0] = o.ua;
    u[o.n-1] = o.ub;

    std::cout << "Working...\n";

    t0 = omp_get_wtime();
    init_rho(rho,o);
    jacobi(u,rho,o);
    t1 = omp_get_wtime();

    output(u,rho,o.n);

    std::cout << "Total time taken: " << t1-t0 << ".\n";
    return 0;
}

void init_rho(std::vector<double>& rho, const struct Opts o)
{
    size_t i;
    #pragma omp parallel for default(none) shared(rho,o) private(i)
    for(i=0; i<o.n; ++i)
        rho[i]=rhox(i*o.dx);
}

void jacobi(std::vector<double>& u, const std::vector<double>& rho, const struct Opts o)
{
    std::vector<double> u0(o.n);
    size_t i,j;
    for(i=0; i<o.its; ++i) {
        #pragma omp parallel for default(none) shared(u0,u,rho,o) private(j)
        for(j=0; j<o.n; ++j)
            u0[j] = u[j];
        #pragma omp parallel for default(none) shared(u0,u,rho,o) private(j)
        for(j=1; j<o.n-1; ++j)
            u[j] = 0.5*(u0[j-1]+u0[j+1]+o.dx*o.dx*rho[j]);
        if(residual(u,rho,o)<=o.tol)
            break;
    }
}

double residual(const std::vector<double>& u, const std::vector<double>& rho, const struct Opts o)
{
    double eqn,res=0;
    size_t j;
    #pragma omp parallel for \
                    default(none) shared(u,rho,o) private(j,eqn) reduction(+:res)
    for(j=1; j<o.n-1; ++j) {
        eqn = u[j-1]-2*u[j]+u[j+1]+o.dx*o.dx*rho[j];
        res += sqrt(eqn*eqn);
    }
    return res;
}

void output(const std::vector<double>& u, const std::vector<double>& rho, const size_t n)
{
    std::ofstream file("poisson_output.dat");
    for(size_t i=0; i<n; ++i)
        file << i << "\t" << u[i] << "\t" << rho[i] << '\n';
}
