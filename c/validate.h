#include <stdlib.h>
#include <math.h>

#ifndef VALIDATE_H
#define VALIDATE_H

/*  Based on serial implementation of function for validation.
 *  Interface is the same as that function, except if function returns value.
 *  In later case, that value is expected at the end.
 *  If successful return 1, otherwise return 0.
 */

//  ex1/2-vector-addition
int validate_vec_add(const size_t, const int * restrict, const int * restrict, const int * restrict);

//  ex3-dot-product
int validate_dot_prod(const size_t, const int * restrict, const int * restrict, const int);

//  ex4-matrix-product
int validate_mat_prod(const size_t, const int * restrict, const int * restrict, const int * restrict);

//  ex5-norm-mean-stddev
int validate_mean(const size_t, const size_t, const double * restrict, const double);
int validate_std(const size_t, const size_t, const double, const double * restrict, const double);

//  ex6-matrix-max
int validate_max(const size_t, const size_t, const int * restrict, const int);

#endif
