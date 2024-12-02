#include <mlp/smalloc.h>
#include <stdio.h>
#include <stdlib.h>

void *smalloc(size_t size) {
    void *ptr = malloc(size);
    if(ptr == NULL) {
        fprintf(stderr, "Malloc of size %lu failed\nExiting...\n", size);
        exit(-1);
    }
    return ptr;
}

void *srealloc(void *ptr, size_t size) {
    void *new_ptr = realloc(ptr, size);
    if(new_ptr == NULL) {
        fprintf(stderr, "Realloc of %p to size %lu failed\nExiting...\n", ptr, size);
        exit(-1);
    }
    return new_ptr;
}

void *scalloc(size_t nmemb, size_t size) {
    void *ptr = calloc(nmemb, size);
    if(ptr == NULL) {
        fprintf(stderr, "Calloc of %lu elements of size %lu failed\nExiting...\n", nmemb, size);
        exit(-1);
    }
    return ptr;
}

void *szalloc(size_t size) {
    return scalloc(1, size);
}

void free2d(void **arr) {
    free(arr[0]);
    free(arr);
}

void **__alloc2d(size_t rows, size_t cols, size_t ptrsize, size_t type_size, void *(alloc)(size_t)) {
    void *arr = alloc(type_size * rows * cols);
    void **ptrarr = smalloc(ptrsize * rows);
    for(size_t i = 0; i < rows; i++) {
        ptrarr[i] = arr + (i * cols * type_size);
    }
    return ptrarr;
}