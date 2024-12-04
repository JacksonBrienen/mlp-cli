#ifndef __SMALLOC_H__
#define __SMALLOC_H__

#include <stddef.h>

/*
 * Safe malloc, exiting if malloc returns NULL
 */
void *smalloc(size_t size);

/*
 * Safe realloc, exiting if realloc returns NULL
 */
void *srealloc(void *prt, size_t size);

/*
 * Safe calloc, exiting if calloc returns NULL
 */
void *scalloc(size_t nmemb, size_t size);

/*
 * Safe zero allocating, exiting if calloc returns NULL
 * Same as calling scalloc(1, size)
 */
void *szalloc(size_t size);

void **__alloc2d(size_t rows, size_t cols, size_t ptrsize, size_t type_size, void *(alloc)(size_t));

/*
 * Frees a 2d array allocated by either szalloc2d or salloc2d
 */
#define free2d(arr) __free2d((void **)(arr));

void __free2d(void **arr);

/*
 * Zero allocates a 2d array of type T with specified rows and cols
 * Allocates in a manner with only 2 alloc calls,
 * this way only 2 memory chunks are created, one of points, and ones of data,
 * so only 2 free calls need to happen on the first array pointer, and on the 2d pointer
 * 
 * Should use free2d to free
 */
#define szalloc2d(T, rows, cols) (T**)__alloc2d(rows, cols, sizeof(T*), sizeof(T), szalloc)

/*
 * Allocates a 2d array of type T with specified rows and cols
 * Allocates in a manner with only 2 alloc calls,
 * this way only 2 memory chunks are created, one of points, and ones of data,
 * so only 2 free calls need to happen on the first array pointer, and on the 2d pointer
 * 
 * Should use free2d to free
 */
#define salloc2d(T, rows, cols) (T**)__alloc2d(rows, cols, sizeof(T*), sizeof(T), smalloc)

#endif