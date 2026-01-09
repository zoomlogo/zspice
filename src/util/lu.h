/**
 * @file lu.h
 */
#pragma once
#include "util/zmth.h"

#include "error.h"
#include "types.h"

/*
#define V_PRINT(v,N) \
    ({ f64 *v_ = (v); const usize N_ = (N); \
       printf("vector(%lu):\n", N_); \
       printf("["); for (usize i = 0; i < N_ - 1; i++) printf("%.3lf ", v_[i]); printf("%.3lf]\n", v_[N_ - 1]); })
#define M_PRINT(m,N) \
    ({ f64 **m_ = (m); const usize N_ = (N); \
       printf("matrix(%lux%lu):\n", N_, N_); \
       for (usize i = 0; i < N_; i++) { \
           printf("["); for (usize j = 0; j < N_ - 1; j++) printf("%.3lf ", m_[i][j]); printf("%.3lf]\n", m_[i][N_ - 1]); \
       }})
*/

// void swap_row(f64 *A, usize N, usize i, usize j);
// void lu_decompose(f64 *A, usize N, usize *p);

// void low_solve(f64 *L, usize N, f64 *B, usize *p);
// void upp_solve(f64 *U, usize N, f64 *Y);

#define LU_TOLERANCE MIN_TOL ///< The max tolerance for a value to be considered 0.

/**
 * @brief Matrix indexing macro.
 *
 * @param a The matrix.
 * @param i The row.
 * @param j The column.
 * @param n The dimension of the matrix.
 */
#define MI(a, i, j, n) ((a)[(n) * (i) + (j)])

/**
 * @brief Linear equation solver for real variables,
 *        using LU Decomposition.
 *
 * Given a linear equation:
 * \f[ A\mathbf x = \mathbf b, \f]
 * we can solve it by rewriting \f(A\f) as a product of two matrices:
 * \f[ A = LU, \f]
 * where \f(L\f) is the lower triangular matrix, and \f(U\f) is the upper triangular matrix.
 *
 * @note The code is generated using the C Preprocessor, and is found in `lu.def`.
 *
 * @param A The LHS matrix.
 * @param N The number of unknowns / Dimension of A.
 * @param B The constant RHS vector.
 */
error_e r_lu_solve(f64 *A, usize N, f64 *B);
/**
 * @brief Linear equation solver for complex variables,
 *        using LU Decomposition.
 *
 * Given a linear equation:
 * \f[ A\mathbf x = \mathbf b, \f]
 * we can solve it by rewriting \f(A\f) as a product of two matrices:
 * \f[ A = LU, \f]
 * where \f(L\f) is the lower triangular matrix, and \f(U\f) is the upper triangular matrix.
 *
 * @note The code is generated using the C Preprocessor, and is found in `lu.def`.
 *
 * @param A The LHS matrix.
 * @param N The number of unknowns / Dimension of A.
 * @param B The constant RHS vector.
 */
error_e c_lu_solve(c64 *A, usize N, c64 *B);
