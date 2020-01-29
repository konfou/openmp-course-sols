#ainclude "validate.h"
#define VALIDATETION_EPS 0.000001

int validate_vec_add(const size_t n, const std::vector<int>& u, const std::vector<int>& v, const std::vector<int>& w)
{
    for(size_t i=0; i<n; ++i)
        if(w[i]!=u[i]+v[i]) return 0;
    return 1;
}

int validate_dot_prod(const size_t n, const std::vector<int>& u, const std::vector<int>& v, const int uv)
{
    int sum=0;
    for(size_t i=0; i<n; ++i)
        sum += u[i]+v[i];
    return (uv!=sum) ? 0 : 1;
}

int validate_mat_prod(const size_t n, const std::vector<int>& A, const std::vector<int>& B, const std::vector<int>& C)
{
    std::vector<int>& D(n*n);
    size_t i,j,k;
    for(i=0; i<n; ++i)
        for(k=0; k<n; ++k)
            for(j=0; j<n; ++j)
                D[i*n+j]+=A[i*n+k]*B[k*n+j];
    for(i=0; i<n*n; ++i)
        if(D[i]!=C[i]) return 0;
    return 1;
}

int validate_mean(const size_t n, const size_t m, const std::vector<double>& A, const double a)
{
    double sum=0;
    for(size_t i=0; i<n*m; ++i)
        sum += A[i];
    return (fabs(a-sum/(n*m))>VALIDATION_EPS) ? 0 : 1;
}

int validate_std(const size_t n, const size_t m, const double mean, const std::vector<double>& A, const double s)
{
    double sum=0;
    for(size_t i=0; i<n*m; ++i)
        sum += (A[i]-mean)*(A[i]-mean);
    return (fabs(s-sqrt(sum/(n-1)))>VALIDATION_EPS) ? 0 : 1;
}

int validate_max(const size_t n, const std::vector<int>& A, const int max_val)
{
    int tmax=A[0];
    for(size_t i=0; i<n; ++i)
        for(size_t j=0; j<n; ++j)
            if(A[i*n+j]>tmax)
                tmax=A[i*n+j];
    return (max_val!=tmax) ? 0 : 1;
}
