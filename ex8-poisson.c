#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>

struct Opts
{
   double a,b,ua,ub,dx,its,tol;
   size_t n;
};

void init_rho(double * restrict, const struct Opts);
void jacobi(double * restrict, const double * restrict, const struct Opts);
double residuals(const double * restrict, const double * restrict, const struct Opts);
void output(const double * restrict, const double * restrict, const struct Opts);

static inline double rhox(const double x)
{
    return sin(1.57*x);
}

int main(void)
{
    struct Opts o;
    double *u,*rho;
    double t0,t1;

    o.a = 0.0;
    o.b = 1.0;
    o.ua = 0.0;
    o.ub = 0.0;
    o.dx = 0.000001;
    o.its = 200000;
    o.tol = 0.00001;

    o.n = (int)(fabs(o.b-o.a)/o.dx);
    u = calloc(o.n,sizeof(double));
    rho = malloc(o.n*sizeof(double));

    u[0] = o.ua;
    u[o.n-1] = o.ub;

    printf("Working...\n");

    t0 = omp_get_wtime();
    init_rho(rho,o);
    jacobi(u,rho,o);
    t1 = omp_get_wtime();

    printf("Total time taken: %f.\n",t1-t0);

    output(u,rho,o);
    free(u);
    free(rho);
    return 0;
}

void init_rho(double * restrict rho, const struct Opts o)
{
    size_t i;
    for(i=0; i<o.n; ++i) rho[i]=rhox(i*o.dx);
}

void jacobi(double * restrict u, const double * restrict rho, const struct Opts o)
{
    double * u0 = malloc(o.n*sizeof(double));
    size_t i,j;
    for(i=0; i<o.its; ++i) {
        for(j=0; j<o.n; ++j)
            u0[j] = u[j];
        for(j=1; j<o.n-1; ++j)
            u[j] = 0.5*(u0[j-1]+u0[j+1]+o.dx*o.dx*rho[j]);
        if(residuals(u,rho,o)<=o.tol)
            break;
    }
    free(u0);
}

double residuals(const double * restrict u, const double * restrict rho, const struct Opts o)
{
    size_t j;
    double eqn,res=0;
    for(j=1; j<o.n-1; ++j) {
        eqn = u[j-1]-2*u[j]+u[j+1]+o.dx*o.dx*rho[j];
        res += sqrt(eqn*eqn);
    }
    return res;
}

void output(const double * restrict u, const double * restrict rho, const struct Opts o)
{
    size_t i;
    FILE *file = fopen("poisson_output.dat","w");
    for(i=0; i<o.n; ++i)
        fprintf(file,"%ld\t%e\t%f\n",i,u[i],rho[i]);
    fclose(file);
}
