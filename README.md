## OpenMP Course Exercises Solutions

Solutions to the exercises found in OpenMP introductory slides from N. Trifonidis,
used during the OpenMP course in MSc Computational Physics at AUTh,
and the exercises set given by N. Stergioulas.

For simplicity in C/C++ versions the matrices are represented as simple 1d arrays/vectors.
Meaning that elements $A_{i,j}$ are accessed:

```
Aij = A[i*n + j]
```

with $0\leq i \lt n, 0\leq j \lt m$, where $n,m$ the number of rows and cols respectively.
Under this, if `A` a `std::vector`, `A.size()` equals `n*m`.

**TODO**: Add validation to f90 files.

**TODO**: Solve and add 2019 homework set.

The slides are available in [[1]], [[2]] (written in Greek).
Also, N. Stergioulas [has a repo][niksterg-openmp-course] with some example programs that introduce OpenMP concepts.

[1]: https://www.astro.auth.gr/~niksterg/courses/progtools/1-OpenMP-tutorial.pdf
[2]: https://www.astro.auth.gr/~niksterg/courses/progtools/2-OpenMP-tutorial.pdf
[niksterg-openmp-course]: https://github.com/niksterg/openmp-course
