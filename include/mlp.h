#ifndef __MLP_H__
#define __MLP_H__

#include "mlp/activations.h"
#include "mlp/linalg.h"
#include "mlp/random.h"
#include "mlp/smalloc.h"
#include <stdio.h>
#include <string.h>
#include <time.h>

typedef struct {
    size_t len;
    size_t *layers;
    matrix_t **weights; // length of len - 1
    activation_t *activations; // length of len - 1

    vector_t **_acts; // length of len
    matrix_t **_derivatives; //length of len - 1
} mlp_t;

typedef struct {
    size_t len;
    vector_t **inputs;
    vector_t **targets;
} dataset_t;

mlp_t *mlp_init(size_t num_layers, size_t* layers, activation_t *activations);

void mlp_free(mlp_t *mlp);

vector_t *__applyf(vector_t *in, float(*f)(float));

vector_t *bias(vector_t *v);

vector_t *forward(mlp_t *mlp, vector_t *inputs, int store);

void backward(mlp_t *mlp, vector_t *error);

void gradient_descent(mlp_t *mlp, float lr);

float mse(vector_t *error);

void train(mlp_t *mlp, dataset_t *data, size_t epochs, float lr);

void predict(mlp_t *mlp, dataset_t *data);

#endif