# MLP CLI
A CLI program for creating, training, and using multilayer perceptrons in C.

Libraries, in both dynamic and static forms, are also available for direct use in a C or C++ program.

## What this project is
This project is a simple cli for using and making MLPs, the mlp structure used is described in [Learning representations by back-propagating errors by Rumelhart, Hinton, and Williams 1986](https://www.nature.com/articles/323533a0). Building off this article means our MLP will learn by using gradient descent and backpropagation. Other techniques, like using [Adam Optimization](https://arxiv.org/abs/1412.6980) may be considered in the future.

## What this project is not
This project is not optimized or "fast." The code for this project can be made better by using parallel computing tools like CUDA and/or OpenCL, and using CPU techniques like SIMD. While I might consider implementing some of these in the future, they will not be included for some time.
 
There are better tools. [PyTorch](https://pytorch.org/) and [TensorFlow](https://www.tensorflow.org/) will simply be more optimized and have more features. If you are looking to do any real computations, this is not the project for you. At its initial state, this project will be simply a "toy" and has no plan to become anything more.