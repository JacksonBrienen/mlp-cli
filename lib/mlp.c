#include <mlp.h>

mlp_t *mlp_init(size_t num_layers, size_t* layers, activation_t *activations) {
    mlp_t *m = smalloc(sizeof(mlp_t));
    m->len = num_layers;
    m->layers = smalloc(sizeof(size_t) * num_layers);
    memcpy(m->layers, layers, sizeof(size_t) * num_layers);

    m->activations = smalloc(sizeof(activation_t) * (num_layers - 1));
    memcpy(m->activations, activations, sizeof(activation_t) * (num_layers - 1));

    m->weights = smalloc(sizeof(matrix_t *) * (num_layers - 1));

    for(size_t i = 0; i < num_layers - 1; i++) {
        size_t rows = layers[i];
        size_t cols = layers[i + 1];
        float **matr = salloc2d(float, rows, cols);
        for(int r = 0; r < rows; r++) {
            for(int c = 0; c < cols; c++) {
                matr[r][c] = randf();
            }
        }
        m->weights[i] = __matrix(rows, cols, matr);
    }

    m->_acts = smalloc(sizeof(vector_t *) * num_layers);
    m->_derivatives = smalloc(sizeof(matrix_t *) * (num_layers - 1));
    return m;
}

void mlp_free(mlp_t *mlp) {
    for(size_t i = 0; i < mlp->len - 1; i++) {
        $matrix(mlp->weights[i]);
    }
    free(mlp->weights);
    free(mlp->layers);
    free(mlp->_acts); // elements of _acts should have already been freed
    free(mlp->_derivatives); // elements of _derivatives should have already been freed
    free(mlp->activations);
    free(mlp);
}

vector_t *__applyf(vector_t *in, float(*f)(float)) {
    vector_t *out = mcopy(in);
    for(int i = 0; i < out->length; i++) {
        out->vector[i] = f(out->vector[i]);
    }
    return out;
}

vector_t *forward(mlp_t *mlp, vector_t *inputs, int store) {
    if(mlp->layers[0] != inputs->length) {
        return NULL;
    }
    vector_t *activations = mcopy(inputs);
    mlp->_acts[0] = mcopy(activations);
    for(size_t i = 0; i < mlp->len - 1; i++) {
        vector_t *net = (vector_t *)mat_mul(activations, mlp->weights[i]);
        $vector(activations);
        activations = __applyf(net, mlp->activations[i].activation);
        $vector(net);
        if(!store) {
            $vector(mlp->_acts[i]);
        }
        mlp->_acts[i + 1] = mcopy(activations);
    }
    
    if(!store) {
        $vector(mlp->_acts[mlp->len - 1]);
    }
    
    return activations;
}

void backward(mlp_t *mlp, vector_t *error) {
    error = mcopy(error);
    for(ssize_t i = mlp->len - 2; i > -1; i--) {
        vector_t *derv = __applyf(mlp->_acts[i+1], mlp->activations[i].derivative);
        vector_t *delta = dot(error, derv);
        $vector(derv);

        $vector(mlp->_acts[i + 1]);

        matrix_t *aT = transpose(mlp->_acts[i]);
        mlp->_derivatives[i] = mat_mul(aT, delta);
        $matrix(aT);

        matrix_t *wT = transpose(mlp->weights[i]);
        $vector(error);
        error = (vector_t *)mat_mul(delta, wT);
        $vector(delta);
        $matrix(wT);
    }
    $vector(error);
    $vector(mlp->_acts[0]);
}

void gradient_descent(mlp_t *mlp, float lr) {
    for(size_t i = 0; i < mlp->len - 1; i++) {
        matrix_t *derivatives = scale(lr, mlp->_derivatives[i]);
        $matrix(mlp->_derivatives[i]);
        matrix_t *weights = mat_add(mlp->weights[i], derivatives);
        $matrix(mlp->weights[i]);
        $matrix(derivatives);
        mlp->weights[i] = weights;
    }
}

float mse(vector_t *error) {
    vector_t *errsq = dot(error, error);
    double res = sum(errsq) / errsq->length;
    $vector(errsq);
    return res;
}

void train(mlp_t *mlp, dataset_t *data, size_t epochs, float lr) {
    for(size_t epoch = 1; epoch < epochs + 1; epoch++) {
        double error_sum = 0;
        for(int i = 0; i < data->len; i++) {
            vector_t *out = forward(mlp, data->inputs[i], 1);
            vector_t *error = (vector_t *)mat_sub(data->targets[i], out);
            backward(mlp, error);
            gradient_descent(mlp, lr);
            error_sum += mse(error);

            $vector(out);
            $vector(error);
        }

        printf("MSE: %f at epoch %i\n", error_sum / data->len, epoch);
    }

}