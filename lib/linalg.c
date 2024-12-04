#include <mlp/linalg.h>
#include <mlp/smalloc.h>
#include <string.h>

/*
 * fills a float ** with values from either a float[][] or float**
 */
#define fill2d(rows, cols, m, arr) do {\
    for(size_t r = 0; r < rows; r++) {\
        memcpy(arr[r], (*(m))[r], sizeof(float) * cols);\
    }\
} while(0)

/*
 * Internal matrix initializer using a pre-allocated 2d float array
 */
matrix_t *__matrix(size_t rows, size_t cols, float **m) {
    if(rows == 1) {
        vector_t *vec = smalloc(sizeof(vector_t));
        vec->_ = 1;
        vec->__ = m;
        vec->vector = m[0];
        vec->length = cols;
        return (matrix_t *)vec;
    }
    matrix_t *mat = smalloc(sizeof(matrix_t));
    mat->rows = rows;
    mat->cols = cols;
    mat->matrix = m;
    return mat;
}

matrix_t *matrix(size_t rows, size_t cols, float m[rows][cols]) {
    float **mat = salloc2d(float, rows, cols);
    fill2d(rows, cols, &m, mat);
    return __matrix(rows, cols, mat);
}

void $matrix(matrix_t *m) {
    free2d(m->matrix);
    free(m);
}

bool __is_vector(matrix_t *m) {
    return m->rows == 1;
}

matrix_t *__mat_add(matrix_t *a, matrix_t *b) {
    if(a->rows != b->rows || a->cols != b->cols) {
        return NULL;
    }

    matrix_t *m = mcopy(a);
    for(size_t r = 0; r < a->rows; r++) {
        for(size_t c = 0; c < a->cols; c++) {
            m->matrix[r][c] += b->matrix[r][c];
        }
    }
    return m;
}

matrix_t *__mat_sub(matrix_t *a, matrix_t *b) {
    if(a->rows != b->rows || a->cols != b->cols) {
        return NULL;
    }

    matrix_t *m = mcopy(a);
    for(size_t r = 0; r < a->rows; r++) {
        for(size_t c = 0; c < a->cols; c++) {
            m->matrix[r][c] -= b->matrix[r][c];
        }
    }
    return m;
}

matrix_t *__mat_mul(matrix_t *a, matrix_t *b) {
    if(a->cols != b->rows) {
        return NULL;
    }

    float **mat = szalloc2d(float, a->rows, b->cols);
    
    for(size_t r = 0; r < a->rows; r++) {
        for(size_t c = 0; c < b->cols; c++) {
            for(size_t i = 0; i < a->cols; i++) {
                mat[r][c] += a->matrix[r][i] * b->matrix[i][c];
            }
        }
    }

    return __matrix(a->rows, b->cols, mat);
}

matrix_t *__mcopy(matrix_t *m) {
    float **mat = szalloc2d(float, m->rows, m->cols);
    fill2d(m->rows, m->cols, (&m->matrix), mat);
    return __matrix(m->rows, m->cols, mat);
}

matrix_t *__scale(float scalar, matrix_t *mat) {
    matrix_t *m = mcopy(mat);
    for(size_t r = 0; r < m->rows; r++) {
        for(size_t c = 0; c < m->cols; c++) {
            m->matrix[r][c] *= scalar;
        }
    }
    return m;
}

matrix_t *__transpose(matrix_t *m) {
    float **mat = salloc2d(float, m->cols, m->rows);
    for(size_t r = 0; r < m->rows; r++) {
        for(size_t c = 0; c < m->cols; c++) {
            mat[c][r] = m->matrix[r][c];
        }
    }
    return __matrix(m->cols, m->rows, mat);
}

float __sum(matrix_t *a) {
    float s = 0;
    for(size_t r = 0; r < a->rows; r++) {
        for(size_t c = 0; c < a->cols; c++) {
            s += a->matrix[r][c];
        }
    }
    return s;
}

/* Vector Specific Operations */
vector_t *vector(size_t length, float v[length]) {
    float **mat = salloc2d(float, 1, length);
    memcpy(mat[0], v, length * sizeof(float));
    return (vector_t *)__matrix(1, length, mat);
}

void $vector(vector_t *v) {
    free2d(v->__);
    free(v);
}

vector_t *dot(vector_t *a, vector_t *b) {
    if(!is_vector(a) || !is_vector(b)) {
        return NULL;
    }

    if(a->length != b->length) {
        return NULL;
    }
    
    vector_t *c = mcopy(a);
    for(size_t i = 0; i < a->length; i++) {
        c->vector[i] *= b->vector[i];
    }

    return c;
}