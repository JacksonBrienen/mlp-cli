#ifndef __MLP_ACTIVATIONS_H__
#define __MLP_ACTIVATIONS_H__

typedef struct {
    float(*activation)(float);
    float(*derivative)(float);
} activation_t;

float __sigmoid(float x);
float __sigmoid_derivative(float x);
extern activation_t sig;

float __ReLU(float x);
float __ReLU_derivative(float x);
extern activation_t ReLU;

float __linear(float x);
float __linear_derivative(float x);
extern activation_t linear;

#endif