/*
 * A PCG based random float generator
 */

#ifndef __MLP_RANDOM_H__
#define __MLP_RANDOM_H__

#include <stdint.h>

/*
 * Seeds the random float generator
 */
void srandf(uint64_t seed);

/*
 * Generates a random float between 0 and 1
 */
float randf();

#endif