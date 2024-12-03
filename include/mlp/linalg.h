#ifndef __MLP_LINALG_H__
#define __MLP_LINALG_H__

#include <stdlib.h>
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

matrix_t *matrix(size_t rows, size_t cols, float m[rows][cols]);

vector_t *vector(size_t length, float v[length]);

void $matrix(matrix_t *m);

void $vector(vector_t *v);

#define is_vector(a) (__is_vector((matrix_t *)(a)))
bool __is_vector(matrix_t *m);

#define mat_add(a, b) __mat_add((matrix_t *)a, (matrix_t*)b)
matrix_t *__mat_add(matrix_t *a, matrix_t *b);

#define mat_sub(a, b) __mat_sub((matrix_t *)a, (matrix_t*)b)
matrix_t *__mat_sub(matrix_t *a, matrix_t *b);

#define mat_mul(a, b) (__mat_mul((matrix_t *)(a), (matrix_t *)(b)))
matrix_t *__mat_mul(matrix_t *a, matrix_t *b);

vector_t *dot(vector_t *a, vector_t *b);

double sum(vector_t *a);

#define mcopy(m) (typeof((m)))__mcopy((matrix_t *)(m))
matrix_t *__mcopy(matrix_t *m);

#define scale(scalar, mat) (typeof(mat))__scale(scalar, (matrix_t *)(mat))
matrix_t *__scale(double scalar, matrix_t *mat);

#define transpose(m) __transpose((matrix_t*)(m))
matrix_t *__transpose(matrix_t *m);

#endif