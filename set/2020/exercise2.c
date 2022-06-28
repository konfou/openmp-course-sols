#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>

#define I(x,y) (x)*(Lx-(x))*(y)*(Ly-(y))
#define V(x,y) (x)*(Lx-(x))*(y)*(Ly-(y))
#define S(x,y,t) 0
#define f(x,y,t) 2*c*c*(1+(t)/2)*((y)*(Ly-(y))+(x)*(Lx-(x)))
#define ua(x,y,t) (x)*(Lx-(x))*(y)*(Ly-(y))*(1+(t)/2)

// if S=const. (dS/dt=0) set 0, else set 1
#define St 0

#define DxDx(u,i,j) (u[i-1][j]-2*u[i][j]+u[i+1][j])
#define DyDy(u,i,j) (u[i][j-1]-2*u[i][j]+u[i][j+1])

void output_n(FILE* file, size_t Nx, size_t Ny,
              double t, double* x, double* y, double** u);
void output_a(FILE* file, size_t Nx, size_t Ny, double Lx, double Ly,
              double t, double* x, double* y);

int main(int argc, char **argv)
{
    //conf
    const double Lx=10, Ly=10, T=20, c=1;
    const size_t Nx=40, Ny=40;
    const double dt=0.1, tp[]= {0,5,10,20};
    FILE *file_n = fopen("wave2d_numeric.dat","w");
    FILE *file_a = fopen("wave2d_analytic.dat","w");
    //vars
    const double dx=Lx/(Nx-1),
                 dy=Ly/(Ny-1),
                 cx2=(c*dt/dx)*(c*dt/dx),
                 cy2=(c*dt/dy)*(c*dt/dy),
                 dt2=dt*dt;
    double t=0, t0, t1;
    double *x, *y, **un, **u, **uo;
    size_t i, j, p=0;

    if(argc==2) {
        int nt;
        sscanf(argv[1],"%d",&nt);
        omp_set_num_threads(nt);
    }

    x = (double*)malloc(Nx*sizeof(double));
    y = (double*)malloc(Ny*sizeof(double));

    un = (double**)malloc(Nx*sizeof(double*));  //u^(n+1)
    u = (double**)malloc(Nx*sizeof(double*));   //u^(n)
    uo = (double**)malloc(Nx*sizeof(double*));  //u^(n-1)

    for(i=0; i<Nx; ++i) {
        un[i] = (double*)malloc(Ny*sizeof(double));
        u[i] = (double*)malloc(Ny*sizeof(double));
        uo[i] = (double*)malloc(Ny*sizeof(double));
    }

    printf("Working...\n");
    t0 = omp_get_wtime();

    #pragma omp parallel default(none) \
                         private(i,j) \
                         shared(p,t,tp,x,y,un,u,uo,file_n,file_a)
    {
        #pragma omp for nowait
        for(i=0; i<Nx; ++i)
            x[i] = i*dx;

        #pragma omp for
        for(j=0; j<Ny; ++j)
            y[j] = j*dy;

        #pragma omp for
        for(i=0; i<Nx; ++i) {
            u[i][0] = S(x[i],y[0],t);
            u[i][Ny-1] = S(x[i],y[Ny-1],t);
        }

        #pragma omp for
        for(j=0; j<Ny; ++j) {
            u[0][j] = S(x[0],y[j],t);
            u[Nx-1][j] = S(x[Nx-1],y[j],t);
        }

        #pragma omp for collapse(2)
        for(i=1; i<Nx-1; ++i)
            for(j=1; j<Ny-1; ++j)
                u[i][j] = I(x[i],y[j]);

        #pragma omp single
        {
            if(fabs(t-tp[p])<dt/2) {
                output_n(file_n,Nx,Ny,t,x,y,u);
                output_a(file_a,Nx,Ny,Lx,Ly,t,x,y);
                ++p;
            }
        }

        #pragma omp for collapse(2)
        for(i=1; i<Nx-1; ++i)
            for(j=1; j<Ny-1; ++j)
                un[i][j] = u[i][j]
                           +dt*V(x[i],y[j])
                           +0.5*cx2*DxDx(u,i,j)
                           +0.5*cy2*DyDy(u,i,j)
                           +0.5*dt2*f(x[i],y[j],t);

        #pragma omp single
        t += dt;

        #pragma omp for
        for(i=0; i<Nx; ++i) {
            un[i][0] = S(x[i],y[0],t);
            un[i][Ny-1] = S(x[i],y[Ny-1],t);
        }

        #pragma omp for
        for(j=0; j<Ny; ++j) {
            un[0][j] = S(x[0],y[j],t);
            un[Nx-1][j] = S(x[Nx-1],y[j],t);
        }

        while(t<T) {
            #pragma omp for collapse(2)
            for(i=0; i<Nx; ++i)
                for(j=0; j<Ny; ++j) {
                    uo[i][j] = u[i][j];
                    u[i][j] = un[i][j];
                }

            #pragma omp for collapse(2)
            for(i=1; i<Nx-1; ++i)
                for(j=1; j<Ny-1; ++j)
                    un[i][j] = 2*u[i][j]
                               -uo[i][j]
                               +cx2*DxDx(u,i,j)
                               +cy2*DyDy(u,i,j)
                               +dt2*f(x[i],y[j],t);

            #pragma omp single
            t += dt;
#if St
            #pragma omp for
            for(i=0; i<Nx; ++i) {
                un[i][0] = S(x[i],y[0],t);
                un[i][Ny-1] = S(x[i],y[Ny-1],t);
            }

            #pragma omp for
            for(j=0; j<Ny; ++j) {
                un[0][j] = S(x[0],y[j],t);
                un[Nx-1][j] = S(x[Nx-1],y[j],t);
            }
#endif
            #pragma omp single
            {
                if(fabs(t-tp[p])<dt/2) {
                    output_n(file_n,Nx,Ny,t,x,y,un);
                    output_a(file_a,Nx,Ny,Lx,Ly,t,x,y);
                    ++p;
                }
            }
        }
    }

    t1 = omp_get_wtime();
    printf("Time taken:   %f.\n",t1-t0);
    printf("Center value: %f.\n",u[Nx/2][Ny/2]);

    fclose(file_n);
    fclose(file_a);

    free(x);
    free(y);
    for(i=0; i<Nx; ++i) {
        free(un[i]);
        free(u[i]);
        free(uo[i]);
    }
    free(un);
    free(u);
    free(uo);

    return 0;
}

void output_n(FILE* file, size_t Nx, size_t Ny,
              double t, double* x, double* y, double** u)
{
    for(size_t i=0; i<Nx; ++i)
        for(size_t j=0; j<Ny; ++j)
            fprintf(file,"%f\t%f\t%f\t%f\n",t,x[i],y[j],u[i][j]);
}

void output_a(FILE* file, size_t Nx, size_t Ny, double Lx, double Ly,
              double t, double* x, double* y)
{
    for(size_t i=0; i<Nx; ++i)
        for(size_t j=0; j<Ny; ++j)
            fprintf(file,"%f\t%f\t%f\t%f\n",t,x[i],y[j],ua(x[i],y[j],t));
}
