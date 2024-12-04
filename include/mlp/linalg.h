#ifndef __MLP_LINALG_H__
#define __MLP_LINALG_H__

#include <stdlib.h>
#include <stdbool.h>
#include "smalloc.h"

/*
 * Matrix struct holds a matrix, usually in heap memory and created by built in functions.
 * Any matrix create where rows = 1, should be created as a vector_t
 */
typedef struct {
    size_t rows;
    size_t cols;
    float **matrix;
} matrix_t;

/*
 * Vector struct is an alternative representation of the matrix struct
 * The _ entry, which refers to the rows entry of matrix, must always be one
 * The vector entry must point to the first element of the __, which is the matrix entry of matrix
 */
typedef struct {
    size_t _;
    size_t length;
    float **__;
    float *vector;
} vector_t;

/* Matrix and Vector Operations */

matrix_t *__matrix(size_t rows, size_t cols, float **m);

/*
 * @returns Heap allocates a matrix with the given rows, cols, and the values within m
 * 
 * If there is a single row, a vector_t will be allocated
 */
matrix_t *matrix(size_t rows, size_t cols, float m[rows][cols]);

/*
 * Frees a heap allocated matrix that was allocated with the matrix(3) function
 */
void $matrix(matrix_t *m);

/*
 * determines whether a is a valid vector_t
 * @return true if a is a vector_t; otherwise false
 */
#define is_vector(a) (__is_vector((matrix_t *)(a)))

/*
 * Internal function to determine is m is a vector_t
 * This function should not be directly called, instead use is_vector(1)
 */
bool __is_vector(matrix_t *m);

/*
 * Performs matrix addition on a and b,
 * @return NULL if a and b are not the same shape i.e. a->rows != b->rows or a->cols != b->cols.
 * If a and b are the same shape, returns a newly allocated matrix_t
 */
#define mat_add(a, b) __mat_add((matrix_t *)a, (matrix_t*)b)

/*
 * Internal function to perform matrix addition
 * This function should not be directly called, instead use mat_add(2)
 */
matrix_t *__mat_add(matrix_t *a, matrix_t *b);

/*
 * Performs matrix subtraction on a and b,
 * @return NULL if a and b are not the same shape i.e. a->rows != b->rows or a->cols != b->cols.
 * If a and b are the same shape, returns a newly allocated matrix_t
 */
#define mat_sub(a, b) __mat_sub((matrix_t *)a, (matrix_t*)b)

/*
 * Internal function to perform matrix subtraction
 * This function should not be directly called, instead use mat_sub(2)
 */
matrix_t *__mat_sub(matrix_t *a, matrix_t *b);

/*
 * Performs matrix multiplication on a and b,
 * @return NULL if a and b are not multipliable i.e. a->cols != b->rows.
 * If a and b are multipliable, returns a newly allocated matrix_t
 */
#define mat_mul(a, b) (__mat_mul((matrix_t *)(a), (matrix_t *)(b)))

/*
 * Internal function to perform matrix multiplication
 * This function should not be directly called, instead use mat_mul(2)
 */
matrix_t *__mat_mul(matrix_t *a, matrix_t *b);

/*
 * @return a newly allocated matrix_t or vector_t with the same values as m
 */
#define mcopy(m) (typeof((m)))__mcopy((matrix_t *)(m))

/*
 * Internal function to copy a matrix.
 * This function should not be directly called, instead use mcopy(1)
 */
matrix_t *__mcopy(matrix_t *m);

/*
 * Performs scalar multiplcation
 * @return a newly allocated matrix_t or vector_t with values multipled by scalar
 */
#define scale(scalar, mat) (typeof(mat))__scale(scalar, (matrix_t *)(mat))

/*
 * Internal function to perform matrix scalar multiplication
 * This function should not be directly called, instead use scale(2)
 */
matrix_t *__scale(float scalar, matrix_t *mat);

/*
 * @return the transpose of m
 */
#define transpose(m) __transpose((matrix_t*)(m))

/*
 * Internal function to calculate a transpose
 * This function should not be directly called, instead use transpose(1)
 */
matrix_t *__transpose(matrix_t *m);

/*
 * @return the sum of all values held by a
 */
#define sum(a) __sum((matrix_t*)(a))

/*
 * Internal function to perform matrix sum
 * This should not be directly called instead call sum(1)
 */
float __sum(matrix_t *a);

/* Vector Specific Operations */

/*
 * Initializes a new vector_t with the given length and values of v
 * @return newly allocated vector_t
 */
vector_t *vector(size_t length, float v[length]);

/*
 * Frees a vector_t
 */
void $vector(vector_t *v);

/*
 * Performs the dot product of two vectors
 * @returns
 * NULL if a or b are not a vector
 * 
 * NULL if a or b are not of the same length
 * 
 * A newly allocated vector_t with the resulting dot product
 */
vector_t *dot(vector_t *a, vector_t *b);

#endif