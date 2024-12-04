#include <math.h>
#include <mlp/activations.h>

float __sigmoid(float x) {
    return (float)(1 / (1 + exp(-x)));
}

float __sigmoid_derivative(float x) {
    return x * (1 - x);
}

float __ReLU(float x) {
    if(x < 0) {
        return 0;
    }
    return x;
}

float __ReLU_derivative(float x) {
    if(x < 0) {
        return 0;
    }
    return 1;
}

float __linear(float x) {
    return x;
}

float __linear_derivative(float x) {
    return 1;
}

activation_t sig = (activation_t){__sigmoid, __sigmoid_derivative};

activation_t ReLU = (activation_t){__ReLU, __ReLU_derivative};

activation_t linear = (activation_t){__linear, __linear_derivative};