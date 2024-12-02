#ifndef __MLP_LINALG_H__
#define __MLP_LINALG_H__

#include <stddef.h>

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

// TODO

#endif