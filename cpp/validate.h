#include <cstdlib>
#include <cmath>
#include <vector>

#ifndef VALIDATE_H
#define VALIDATE_H

/*  Based on serial implementation of function for validation.
 *  Interface is the same as that function, except if function returns value.
 *  In later case, that value is expected at the end.
 *  If successful return 1, otherwise return 0.
 */

//  ex1/2-vector-addition
int validate_vec_add(const std::vector<int>& u,
                     const std::vector<int>& v,
                     const std::vector<int>& w);

//  ex3-dot-product
int validate_dot_prod(const std::vector<int>& u,
                      const std::vector<int>& v, const int uv);

//  ex4-matrix-product
int validate_mat_prod(const size_t, const std::vector <int>& A,
                      const std::vector<int>& B,
                      const std::vector<int>& C);

//  ex5-norm-mean-stddev
int validate_mean(const std::vector<double>& A, const double a);
int validate_std(const double mean, const std::vector<double>& A,
                 const double s);

//  ex6-matrix-max
int validate_max(const size_t n, const size_t m,
                 const std::vector<int>& A, const int max_val);

#endif
